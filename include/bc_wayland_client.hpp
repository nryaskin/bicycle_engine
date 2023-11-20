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

    void RegistryGlobal(void *data,
                        bicycle_engine::wl_registry registry,
                        uint32_t name,
                        const char *interface,
                        uint32_t version);

    void RegistryGlobalRemove(void *data,
                              bicycle_engine::wl_registry registry,
                              uint32_t name);

    void XDGSurfaceConfigure(void *data,
                             struct xdg_surface *xdg_surface,
                             uint32_t serial);

    void Ping(void *data,
              struct xdg_wm_base *xdg_wm_base,
              uint32_t serial);

	void Release(void *data, struct wl_buffer *wl_buffer);

    void Format(void *data, struct wl_shm *wl_shm, uint32_t format);

	void FrameDone(void *data, struct wl_callback *wl_callback, uint32_t callback_data);

    void Geometry(void *data,
                  struct wl_output *wl_output,
                  int32_t x,
                  int32_t y,
                  int32_t physical_width,
                  int32_t physical_height,
                  int32_t subpixel,
                  const char *make,
                  const char *model,
                  int32_t transform);

    void Mode(void *data,
		      struct wl_output *wl_output,
		      uint32_t flags,
		      int32_t width,
		      int32_t height,
		      int32_t refresh);

    void Done(void *data, struct wl_output *wl_output);

    void Scale(void *data,
		       struct wl_output *wl_output,
		       int32_t factor);

    void Name(void *data, struct wl_output *wl_output, const char *name);

    void Description(void *data, struct wl_output *wl_output, const char *description);

    class WaylandClient : public GraphicsDevice<uint32_t> {
    public:
        WaylandClient(); 
        void SetTitle(std::string&& title);
        void Clear(uint32_t color) override;
        void DrawPixel(int x, int y, uint32_t color) override;
        void Dispatch();
        ~WaylandClient(); 

        friend void RegistryGlobal(void *data,
                                   wl_registry registry,
                                   uint32_t name,
                                   const char *interface,
                                   uint32_t version);

        friend void RegistryGlobalRemove(void *data,
                                         wl_registry registry,
                                         uint32_t name);

        friend void XDGSurfaceConfigure(void *data,
                                        struct xdg_surface *xdg_surface,
                                        uint32_t serial);

        friend void Ping(void *data,
                         struct xdg_wm_base *xdg_wm_base,
                         uint32_t serial);

        friend void Format(void *data,
                           struct wl_shm *wl_shm,
		                   uint32_t format);

	    friend void Release(void *data, struct wl_buffer *wl_buffer);

	    friend void FrameDone(void *data, struct wl_callback *wl_callback, uint32_t callback_data);
        friend void Geometry(void *data,
                             struct wl_output *wl_output,
                             int32_t x,
                             int32_t y,
                             int32_t physical_width,
                             int32_t physical_height,
                             int32_t subpixel,
                             const char *make,
                             const char *model,
                             int32_t transform);

        friend void Mode(void *data,
                         struct wl_output *wl_output,
                         uint32_t flags,
                         int32_t width,
                         int32_t height,
                         int32_t refresh);

        friend void Done(void *data, struct wl_output *wl_output);

        friend void Scale(void *data,
                          struct wl_output *wl_output,
                          int32_t factor);

        friend void Description(void *data, struct wl_output *wl_output, const char *description);
    private:
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
            .configure = XDGSurfaceConfigure
        };
        // Global listener
        struct wl_registry_listener m_wc_registry_listener = {
            .global = RegistryGlobal,
            .global_remove = RegistryGlobalRemove
        };

        struct wl_output_listener wl_output_listener = {
            .geometry = Geometry,
            .mode = Mode,
            .done = Done,
	        .scale = Scale,
	        .name = Name,
	        .description = Description,
        };

        // WM base listener
        struct xdg_wm_base_listener xdg_wm_base_listener = {
            .ping = Ping,
        };

        // Format listener
        struct wl_shm_listener wl_shm_listener = {
            .format = Format,
        };
        // Buffer listener
        struct wl_buffer_listener wl_buffer_listener = {
            .release = Release,
        };

        // Frame listener
        struct wl_callback_listener wl_surface_frame_listener = {
            .done = FrameDone,
        };

    };

}
#endif /* BICYCLE_ENGINE_WAYLAND_CLIENT_H */
