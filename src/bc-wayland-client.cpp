#include "bc_wayland_client.hpp"
#include "memory/bc_wayland_shm.hpp"
#include "memory/bc_wayland_shm_pool.hpp"
#include "bc_statistics.hpp"
#include <format>
#include <iostream>
#include <cstring>
#include <format>
#include <limits>
#include <source_location>

namespace bcmem = bicycle_engine::wayland::memory;
namespace bicycle_engine {

void _log(std::string& location, std::string_view format_str, std::format_args args) {
    std::cout << location << std::vformat(format_str, args) << "\n";
}

inline void info_log(const std::source_location& loc,
                     std::string_view format_str,
                     auto&& ...args) {
    auto str = std::vformat("{}:{}({})>> ", std::make_format_args(loc.file_name(), loc.line(), loc.function_name()));
    _log(str, format_str, std::make_format_args(args...));
}

// TODO: Try to add async version of this that uses fd from wl_display
//       Use wl_display_get_fd to get file descriptor and when there is activity call wl_event_loop_dispatch.
WaylandClient::WaylandClient() : wc_display(wl_display_connect(NULL), &wl_display_disconnect), height(0), width(0) {
    if (!wc_display) {
        throw FailedToConnectWaylandException();
    }

    registry = wl_display_get_registry(wc_display.get());
    if (!registry) {
        throw FailedToGetRegistryException();
    }

    wl_registry_add_listener(registry, &m_wc_registry_listener, this);
    // NOTE: We are blocking here until all pending request are processed by the server.
    // We can do it here since we are filling alreadt initialized non-static members.
    wl_display_roundtrip(wc_display.get());

    surface = wl_compositor_create_surface(compositor);

    xdg_surface = xdg_wm_base_get_xdg_surface(xdg_wm_base, surface);

    xdg_surface_add_listener(xdg_surface, &xdg_surface_listener, this);
    xdg_toplevel = xdg_surface_get_toplevel(xdg_surface);

    wl_surface_commit(surface);

    frame_cb = wl_surface_frame(surface);
    wl_callback_add_listener(frame_cb, &wl_surface_frame_listener, this);

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
    bicycle_engine::info_log(std::source_location::current(),
                             "interface: {}, version {}, name: {}",
                             interface, version, name);

    WaylandClient* wc = static_cast<WaylandClient*>(data);

    if (std::strcmp(interface, wl_shm_interface.name) == 0) {
        wc->wl_shm = static_cast<struct wl_shm*>(wl_registry_bind(wc->registry,
                                              name,
                                              &wl_shm_interface,
                                              1));
        wl_shm_add_listener(wc->wl_shm, &wc->wl_shm_listener, wc);
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
    } else if (std::strcmp(interface, wl_output_interface.name) == 0) {
        wc->output = static_cast<struct wl_output*>(wl_registry_bind(wc->registry,
                                              name,
                                              &wl_output_interface,
                                              4));
        wl_output_add_listener(wc->output, &wc->wl_output_listener, wc);
    }
}

void RegistryGlobalRemove(void *data,
                          bicycle_engine::wl_registry registry,
                          uint32_t name) {

}

void XDGSurfaceConfigure(void *data,
                         struct xdg_surface *xdg_surface,
                         uint32_t serial) {
    bicycle_engine::info_log(std::source_location::current(), "serial: {}", serial);
    WaylandClient* wc = static_cast<WaylandClient*>(data);
    xdg_surface_ack_configure(wc->xdg_surface, serial);

    struct wl_buffer* buffer = wc->get_frame();
    wl_surface_attach(wc->surface, buffer, 0, 0);
    wl_surface_commit(wc->surface);
}

void Ping(void *data, struct xdg_wm_base *xdg_wm_base, uint32_t serial) {
    xdg_wm_base_pong(xdg_wm_base, serial);
}

void Format(void *data, struct wl_shm *wl_shm, uint32_t format) {
    bicycle_engine::info_log(std::source_location::current(), "Received format: {:#X}", format);
    WaylandClient* wc = static_cast<WaylandClient*>(data);
    wc->data_formats.insert(static_cast<wl_shm_format>(format));
}

void Release(void *data, struct wl_buffer *wl_buffer) {
    wl_buffer_destroy(wl_buffer);
}
                     
void FrameDone(void *data, struct wl_callback *wl_callback, uint32_t callback_data) {
    bicycle_engine::info_log(std::source_location::current(), "");
    WaylandClient* wc = static_cast<WaylandClient*>(data);
    wl_callback_destroy(wc->frame_cb);

    // Replace it with Frame Callback type
    // So next callback would look like
    // frame_cb = FrameCallback(listener, this);
    // This way the previous one goes out of scope and destroyed and the new one created.
    wc->request_new_frame_callback();
    wc->stats.add_frame();

    auto buffer = wc->get_frame();

    wl_buffer_add_listener(buffer, &wc->wl_buffer_listener, wc);
    //NOTE: It is not necessary to attach each time
    wl_surface_attach(wc->surface, buffer, 0, 0);
    // TODO: Compute damaged area so there is not need to repaint full windows
    wl_surface_damage_buffer(wc->surface,
                             0, 0,
                             std::numeric_limits<int32_t>::max(),
                             std::numeric_limits<int32_t>::max());
    wl_surface_commit(wc->surface);
    bicycle_engine::info_log(std::source_location::current(), "Frames: {}", wc->stats.get_statistics());
}

void Geometry(void *data,
              struct wl_output *wl_output,
              int32_t x,
              int32_t y,
              int32_t physical_width,
              int32_t physical_height,
              int32_t subpixel,
              const char *make,
              const char *model,
              int32_t transform) {
    bicycle_engine::info_log( std::source_location::current(), "x: {}, y: {}, physical_width: {}, physical_height: {}" , x, y, physical_height, physical_width);
}

void Mode(void *data,
	      struct wl_output *wl_output,
	      uint32_t flags,
	      int32_t width,
	      int32_t height,
	      int32_t refresh) {
    bicycle_engine::info_log( std::source_location::current(), "flags: {:#X}, width: {}, height: {}, refresh: {}" , flags, width, height, refresh);
    // Warning: This is not exactly correct solution but I am going to stick with this before refactoring to get MVP
    auto wc = static_cast<bicycle_engine::WaylandClient *>(data);
    wc->height = height;
    wc->width = width;
}

void Done(void *data, struct wl_output *wl_output) {
    bicycle_engine::info_log( std::source_location::current(), "");
    auto wc = static_cast<bicycle_engine::WaylandClient *>(data);
    wc->shared_mem.resize(wc->height * wc->width * 4);
}

void Scale(void *data,
	       struct wl_output *wl_output,
	       int32_t factor) {
    bicycle_engine::info_log( std::source_location::current(), "factor: {}", factor);
}

void Name(void *data, struct wl_output *wl_output, const char *name) {
    bicycle_engine::info_log( std::source_location::current(), "name: {}", name);
}

void Description(void *data, struct wl_output *wl_output, const char *description) {
    bicycle_engine::info_log( std::source_location::current(), "description: {}", description);
}

// TODO: Return buffer which can be damaged
struct wl_buffer* WaylandClient::get_frame() {
    int stride = width * 4;
    int size = stride * height;

    auto shared_memory_pool = bcmem::SharedMemoryPool(wl_shm, shared_mem);
    auto buffer = shared_memory_pool.create_buffer(0,
                                                   width,
                                                   height,
                                                   stride,
                                                   WL_SHM_FORMAT_XRGB8888);
    return buffer;
}

void WaylandClient::request_new_frame_callback() {
    // TODO: Maybe protect with mutext so it can be scheduled only once.
    frame_cb = wl_surface_frame(surface);
    wl_callback_add_listener(frame_cb, &wl_surface_frame_listener, this);
}

void WaylandClient::Clear(uint32_t color) {
    //throw std::runtime_error("Not implemented yet!");
}

void WaylandClient::DrawPixel(int x, int y, uint32_t color) {
    throw std::runtime_error("Not implemented yet");
}

void WaylandClient::Dispatch() {
    wl_display_dispatch(wc_display.get());
}

WaylandClient::~WaylandClient() {
    bicycle_engine::info_log(std::source_location::current(),
                             "FPS is: {}", stats.get_statistics());
}
}
