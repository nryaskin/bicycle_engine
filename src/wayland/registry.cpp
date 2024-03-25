#include "wayland/registry.hpp"

#include <format>
#include <iostream>
#include <cstring>
#include <format>
#include <limits>
#include <source_location>

#include <cassert>

#include "bc_statistics.hpp"
#include "logger/logger.hpp"


namespace belog = bicycle_engine::logger;

namespace bicycle_engine::wayland {
Registry::Registry() :
    registry(nullptr, wl_registry_destroy),
    compositor(nullptr, wl_compositor_destroy),
    xdg_wm_base(nullptr, xdg_wm_base_destroy) {}

Registry::Registry(native_ptr_t wl_registry, std::map<std::string, user_registry_cb_t> cbs) :
    registry(wl_registry, wl_registry_destroy),
    compositor(nullptr, wl_compositor_destroy),
    xdg_wm_base(nullptr, xdg_wm_base_destroy),
    registry_listener(new registry_listener_t),
    user_registry_cbs(cbs) {

    registry_listener->global = Registry::registry_global;
    registry_listener->global_remove = Registry::registry_global_remove;

    Registry::set_registry(registry.get(), this);
    wl_registry_add_listener(registry.get(), registry_listener.get(), nullptr);
}

Registry::Registry(Registry&& other) :
    registry(nullptr, wl_registry_destroy),
    compositor(nullptr, wl_compositor_destroy),
    xdg_wm_base(nullptr, xdg_wm_base_destroy) {
    *this = std::move(other);
}

Registry& Registry::operator=(Registry&& other) {
    std::swap(registry, other.registry);
    if (registry) {
        Registry::set_registry(registry.get(), this);
    }
    if (other.registry) {
        Registry::set_registry(other.registry.get(), &other);
    }

    std::swap(compositor, other.compositor);
    std::swap(xdg_wm_base, other.xdg_wm_base);
    std::swap(go_cache, other.go_cache);
    std::swap(registry_listener, other.registry_listener);
    std::swap(xdg_wm_base_listener, other.xdg_wm_base_listener);
    std::swap(user_registry_cbs, other.user_registry_cbs);


    return *this;
}

void Registry::set_registry(native_ptr_t wl_registry, Registry* registry) {
    assert(wl_registry);
    wl_registry_set_user_data(wl_registry, registry);
}

Registry* Registry::get_registry(native_ptr_t wl_registry) {
    assert(wl_registry);
    return static_cast<Registry *>(wl_registry_get_user_data(wl_registry));
}

void Registry::registry_global(void *data,
                               native_ptr_t wl_registry,
                               uint32_t name,
                               const char *interface,
                               uint32_t version) {

    auto registry = get_registry(wl_registry);

    if (std::strcmp(interface, wl_compositor_interface.name) == 0) {
        auto r = registry->bind<struct wl_compositor, decltype(&wl_compositor_destroy)>(name, &wl_compositor_interface, version, wl_compositor_destroy);
        registry->compositor.swap(r);
    } else if (std::strcmp(interface, xdg_wm_base_interface.name) == 0) {
        auto xdg_wm_base = registry->bind<struct xdg_wm_base, decltype(&xdg_wm_base_destroy)>(name, &xdg_wm_base_interface, version, xdg_wm_base_destroy);
        registry->xdg_wm_base.swap(xdg_wm_base);
        xdg_wm_base_add_listener(registry->xdg_wm_base.get(), &registry->xdg_wm_base_listener, registry);
    } else {
        registry->go_cache.add(interface, name, version);
        if (auto cb = registry->user_registry_cbs.find(interface); cb != registry->user_registry_cbs.end()) {
            if (std::strcmp(cb->first.c_str(), wl_output_interface.name) == 0) {
                cb->second(*registry, name, &wl_output_interface, version);
            }
        }
    }
}

void Registry::registry_global_remove(void *data,
                                      native_ptr_t wl_registry,
                                      uint32_t name) {
    bicycle_engine::logger::Logger("name: {}", name);
    Registry * registry = static_cast<Registry *>(data);

    // Here before if we can block by mutex
    //TODO: Send remove to created object. I think for this I will need to do some internal bind
    registry->go_cache.remove(name);
}

void Registry::ping(void *data, struct xdg_wm_base *xdg_wm_base, uint32_t serial) {
    xdg_wm_base_pong(xdg_wm_base, serial);
}

Registry::~Registry() {}

}
