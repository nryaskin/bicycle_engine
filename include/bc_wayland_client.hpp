#ifndef BICYCLE_ENGINE_WAYLAND_CLIENT_H
#define BICYCLE_ENGINE_WAYLAND_CLIENT_H
#include <wayland-client.h>
#include <memory>
#include <set>
#include "bicycle_engine.hpp"
#include "memory/bc_wayland_shm.hpp"
#include "bc_statistics.hpp"
#include "xdg-shell-client-protocol.h"

namespace bicycle_engine {

    using wl_registry = struct wl_registry *;
    class FailedToConnectWaylandException : public std::runtime_error {
    public:
        FailedToConnectWaylandException() : std::runtime_error::runtime_error("Failed to connect to Wayland display") {}
    };

    class FailedToGetRegistryException : public std::runtime_error {
    public:
        FailedToGetRegistryException() : std::runtime_error::runtime_error("Failed to Wayland registry") {}
    };

    class DisplayDeleter {
    public:
        DisplayDeleter() = default;
        void operator()(struct wl_display *display) {
            wl_display_disconnect(display);
        }
    };


    class WaylandClient : public GraphicsDevice<uint32_t> {
    public:
        WaylandClient(); 
        void set_title(std::string&& title);
        void clear(uint32_t color) override;
        void draw_pixel(int x, int y, uint32_t color) override;
        void dispatch();
        ~WaylandClient(); 

    private:

        static void registry_global(void *data,
                                    bicycle_engine::wl_registry registry,
                                    uint32_t name,
                                    const char *interface,
                                    uint32_t version);

        static void registry_global_remove(void *data,
                                           bicycle_engine::wl_registry registry,
                                           uint32_t name);

        static void xdg_surface_configure(void *data,
                                          struct xdg_surface *xdg_surface,
                                          uint32_t serial);

        static void ping(void *data,
                         struct xdg_wm_base *xdg_wm_base,
                         uint32_t serial);

        static void release(void *data, struct wl_buffer *wl_buffer);

        static void format(void *data, struct wl_shm *wl_shm, uint32_t format);

        static void frame_done(void *data, struct wl_callback *wl_callback, uint32_t callback_data);

        static void geometry(void *data,
                             struct wl_output *wl_output,
                             int32_t x,
                             int32_t y,
                             int32_t physical_width,
                             int32_t physical_height,
                             int32_t subpixel,
                             const char *make,
                             const char *model,
                             int32_t transform);

        static void mode(void *data,
                         struct wl_output *wl_output,
                         uint32_t flags,
                         int32_t width,
                         int32_t height,
                         int32_t refresh);

        static void done(void *data, struct wl_output *wl_output);

        static void scale(void *data,
                          struct wl_output *wl_output,
                          int32_t factor);

        static void name(void *data, struct wl_output *wl_output, const char *name);

        static void description(void *data, struct wl_output *wl_output, const char *description);

        // @brief Draw frame
        struct wl_buffer* get_frame();
        // @brief request new frame callback
        void request_new_frame_callback();

        std::unique_ptr<struct wl_display, decltype(&wl_display_disconnect)> wc_display;
        // Wayland registry
        wl_registry registry;  
        // Shared memory registry interface
        struct wl_shm* wl_shm;
        // Compositor used to create surfaces and regions
        struct wl_compositor* compositor;
        // Seat used to query input devices
        struct wl_seat* seat;
        // Used for wayland ping pong
        struct xdg_wm_base* xdg_wm_base;
        // Wayland surface
        struct wl_surface* surface;
        // XDG surface 
        struct xdg_surface* xdg_surface;
        // XDG top level surface
        struct xdg_toplevel* xdg_toplevel;
        // Frame callback
        struct wl_callback* frame_cb;
        // Wayland Output
        struct wl_output* output;

        // Height
        int height;
        // Width
        int width;
        // Formats set that were provided to Format callback
        std::set<wl_shm_format> data_formats;

        // Shared Memory
        bicycle_engine::wayland::memory::SharedMemory shared_mem;
        // For now will be used for frame counting
        misc::statistics::Statistics stats;
        // Listeners
        // XDG listener
        struct xdg_surface_listener xdg_surface_listener = {
            .configure = WaylandClient::xdg_surface_configure
        };
        // Global listener
        struct wl_registry_listener m_wc_registry_listener = {
            .global        = WaylandClient::registry_global,
            .global_remove = WaylandClient::registry_global_remove
        };

        struct wl_output_listener wl_output_listener = {
            .geometry     = WaylandClient::geometry,
            .mode         = WaylandClient::mode,
            .done         = WaylandClient::done,
	        .scale        = WaylandClient::scale,
	        .name         = WaylandClient::name,
	        .description  = WaylandClient::description,
        };

        // WM base listener
        struct xdg_wm_base_listener xdg_wm_base_listener = {
            .ping = WaylandClient::ping,
        };

        // Format listener
        struct wl_shm_listener wl_shm_listener = {
            .format = WaylandClient::format,
        };
        // Buffer listener
        struct wl_buffer_listener wl_buffer_listener = {
            .release = WaylandClient::release,
        };

        // Frame listener
        struct wl_callback_listener wl_surface_frame_listener = {
            .done = WaylandClient::frame_done,
        };

    };

}
#endif /* BICYCLE_ENGINE_WAYLAND_CLIENT_H */
