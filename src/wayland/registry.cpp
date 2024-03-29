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

std::shared_ptr<Registry> Registry::create(native_ptr_t wl_registry, std::map<std::string, user_registry_cb_t> cbs) {
    return std::shared_ptr<Registry>(new Registry(wl_registry, cbs));
}

Registry::Registry(native_ptr_t wl_registry, std::map<std::string, user_registry_cb_t> cbs) :
    registry(wl_registry, wl_registry_destroy),
    compositor(nullptr, wl_compositor_destroy),
    xdg_wm_base(nullptr, xdg_wm_base_destroy),
    user_registry_cbs(cbs) {

    wl_registry_add_listener(registry.get(), &registry_listener, this);
}

void Registry::registry_global(void *data,
                               native_ptr_t wl_registry,
                               uint32_t name,
                               const char *interface,
                               uint32_t version) {
    auto registry = static_cast<Registry *>(data);

    if (std::strcmp(interface, wl_compositor_interface.name) == 0) {
        auto r = registry->bind<struct wl_compositor, decltype(&wl_compositor_destroy)>(name, &wl_compositor_interface, version, wl_compositor_destroy);
        registry->compositor.swap(r);
    } else if (std::strcmp(interface, xdg_wm_base_interface.name) == 0) {
        auto xdg_wm_base = registry->bind<struct xdg_wm_base, decltype(&xdg_wm_base_destroy)>(name, &xdg_wm_base_interface, version, xdg_wm_base_destroy);
        registry->xdg_wm_base.swap(xdg_wm_base);
        xdg_wm_base_add_listener(registry->xdg_wm_base.get(), &registry->xdg_wm_base_listener, nullptr);
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
    auto registry = static_cast<Registry *>(data);

    // Here before if we can block by mutex
    //TODO: Send remove to created object. I think for this I will need to do some internal bind
    registry->go_cache.remove(name);
}

void Registry::ping(void *data, struct xdg_wm_base *xdg_wm_base, uint32_t serial) {
    xdg_wm_base_pong(xdg_wm_base, serial);
}

Registry::~Registry() {}

}
