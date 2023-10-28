#include "bc_wayland_client.hpp"
#include "bc_wayland_shm.hpp"
#include <format>
#include <iostream>
#include <cstring>
#include <format>
#include <sys/mman.h>

namespace bcmem = bicycle_engine::wayland::memory;
namespace bicycle_engine {

void _log(std::string_view format_str, std::format_args args) {
    std::cout << std::vformat(format_str, args) << "\n";
}

inline void log(std::string_view format_str, auto&& ...args) {
    _log(format_str, std::make_format_args(args...));
}

// TODO: Try to add async version of this that uses fd from wl_display
//       Use wl_display_get_fd to get file descriptor and when there is activity call wl_event_loop_dispatch.
WaylandClient::WaylandClient() : wc_display(wl_display_connect(NULL), DisplayDeleter()), height(480), width(640)  {
    if (!wc_display) {
        throw FailedToConnectWaylandException();
    }

    registry = wl_display_get_registry(wc_display.get());
    if (!registry) {
        throw FailedToGetRegistryException();
    }

    wl_registry_add_listener(registry, &m_wc_registry_listener, this);
    // NOTE: We are blocking here until all pending request are processed by the server.
    wl_display_roundtrip(wc_display.get());


    surface = wl_compositor_create_surface(compositor);
    xdg_surface = xdg_wm_base_get_xdg_surface(xdg_wm_base, surface);
    xdg_surface_add_listener(xdg_surface, &xdg_surface_listener, this);
    xdg_toplevel = xdg_surface_get_toplevel(xdg_surface);
    wl_surface_commit(surface);
}

void WaylandClient::SetTitle(std::string&& title) {
    if (xdg_toplevel) {
        xdg_toplevel_set_title(xdg_toplevel, title.c_str());
    }
}

void RegistryGlobal(void *data,
                    bicycle_engine::wl_registry registry,
                    uint32_t name,
                    const char *interface,
                    uint32_t version) {
    bicycle_engine::log("interface: {}, version {}, name: {}", interface, version, name);

    WaylandClient* wc = static_cast<WaylandClient*>(data);

    if (std::strcmp(interface, wl_shm_interface.name) == 0) {
        wc->wl_shm = static_cast<struct wl_shm*>(wl_registry_bind(wc->registry,
                                              name,
                                              &wl_shm_interface,
                                              1));
    } else if (std::strcmp(interface, wl_compositor_interface.name) == 0) {
        wc->compositor = static_cast<struct wl_compositor*>(wl_registry_bind(wc->registry,
                                                            name,
                                                            &wl_compositor_interface,
                                                            4));
    } else if (std::strcmp(interface, xdg_wm_base_interface.name) == 0) {
        wc->xdg_wm_base = static_cast<struct xdg_wm_base*>(wl_registry_bind(wc->registry,
                                                           name,
                                                           &xdg_wm_base_interface,
                                                           1)); 
        xdg_wm_base_add_listener(wc->xdg_wm_base, &wc->xdg_wm_base_listener, wc);
    } else if (std::strcmp(interface, wl_seat_interface.name) == 0) {
        wc->seat = static_cast<struct wl_seat*>(wl_registry_bind(wc->registry,
                                                name,
                                                &wl_seat_interface,
                                                7));
#if 0
        wl_seat_add_listener(wc->seat, nullptr, wc);
#endif
    }
}

void RegistryGlobalRemove(void *data,
                          bicycle_engine::wl_registry registry,
                          uint32_t name) {

}

void XDGSurfaceConfigure(void *data,
                         struct xdg_surface *xdg_surface,
                         uint32_t serial) {
    WaylandClient* wc = static_cast<WaylandClient*>(data);
    xdg_surface_ack_configure(wc->xdg_surface, serial);

    struct wl_buffer* buffer = wc->draw_frame();
    wl_surface_attach(wc->surface, buffer, 0, 0);
    wl_surface_commit(wc->surface);
}

void Ping(void *data, struct xdg_wm_base *xdg_wm_base, uint32_t serial) {
    xdg_wm_base_pong(xdg_wm_base, serial);
}

void Release(void *data, struct wl_buffer *wl_buffer) {
    wl_buffer_destroy(wl_buffer);
}
                     

struct wl_buffer* WaylandClient::draw_frame() {
    int stride = width * 4;
    int size = stride * height;
    bcmem::SharedMemory shm(size);

    struct wl_shm_pool *pool = wl_shm_create_pool(wl_shm, shm.get_fd(), size);
    struct wl_buffer *buffer = wl_shm_pool_create_buffer(pool, 0,
            width, height, stride, WL_SHM_FORMAT_XRGB8888);
    wl_shm_pool_destroy(pool);

        /* Draw checkerboxed background */
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            if ((x + y / 8 * 8) % 16 < 8)
                shm[y * width + x] = 0xFF666666;
            else
                shm[y * width + x] = 0xFFEEEEEE;
        }
    }

    wl_buffer_add_listener(buffer, &wl_buffer_listener, nullptr);
    return buffer;
}

void WaylandClient::Clear(uint32_t color) {
    throw std::runtime_error("Not implemented yet!");
}

void WaylandClient::DrawPixel(int x, int y, uint32_t color) {
    throw std::runtime_error("Not implemented yet");
}

void WaylandClient::Dispatch() {
    wl_display_dispatch(wc_display.get());
}

WaylandClient::~WaylandClient() {
}
}
