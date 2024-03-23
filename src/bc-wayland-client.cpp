#include <format>
#include <iostream>
#include <cstring>
#include <format>
#include <limits>
#include <source_location>

#include "bc_wayland_client.hpp"
#include "bc_statistics.hpp"
#include "logger/logger.hpp"


namespace belog = bicycle_engine::logger;

namespace bicycle_engine::wayland {

WaylandClient::WaylandClient() : registry(display.get_registry()) {


    if (!registry) {
        throw FailedToGetRegistryException();
    }

    wl_registry_add_listener(registry.get(), &m_wc_registry_listener, this);

    display.roundtrip();
}

void WaylandClient::registry_global(void *data,
                                    wl_registry_t *registry,
                                    uint32_t name,
                                    const char *interface,
                                    uint32_t version) {
#if 0
    bicycle_engine::logger::Logger("interface: {}, version {}, name: {}", interface, version, name);
#endif

    WaylandClient* wc = static_cast<WaylandClient*>(data);

    if (std::strcmp(interface, wl_compositor_interface.name) == 0) {
        wc->compositor = static_cast<struct wl_compositor*>(wl_registry_bind(wc->registry.get(),
                                                            name,
                                                            &wl_compositor_interface,
                                                            4));
    } else if (std::strcmp(interface, xdg_wm_base_interface.name) == 0) {
        wc->xdg_wm_base = static_cast<struct xdg_wm_base*>(wl_registry_bind(wc->registry.get(),
                                                           name,
                                                           &xdg_wm_base_interface,
                                                           1)); 
        xdg_wm_base_add_listener(wc->xdg_wm_base, &wc->xdg_wm_base_listener, wc);
    } else {
        wc->go_cache.add(interface, name, version);
    }
}

void WaylandClient::registry_global_remove(void *data,
                                           wl_registry_t *registry,
                                           uint32_t name) {
    bicycle_engine::logger::Logger("name: {}", name);
    WaylandClient* wc = static_cast<WaylandClient*>(data);

    wc->go_cache.remove(name);
}

void WaylandClient::ping(void *data, struct xdg_wm_base *xdg_wm_base, uint32_t serial) {
    xdg_wm_base_pong(xdg_wm_base, serial);
}

surface::WLSurface WaylandClient::create_toplevel_surface() {
    return surface::WLSurface(compositor, xdg_wm_base);
}


void WaylandClient::roundtrip() {
    if (display.roundtrip() < 0) {
        throw std::runtime_error("Couldn't process display queue in roundtirp, not handled!");
    }
}

void WaylandClient::dispatch() {
    if (display.dispatch() < 0) {
        throw std::runtime_error("Couldn't process display queue, not handled!");
    }
}

WaylandClient::~WaylandClient() {
}
}
