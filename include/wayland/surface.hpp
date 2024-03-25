#ifndef BC_WAYLAND_SURFACE_H
#define BC_WAYLAND_SURFACE_H

#include <functional>
#include <optional>

#include "xdg_client_header.h"

#include "bc_wayland_frame_callback.hpp"
#include "bc_statistics.hpp"
#include "wayland/display.hpp"
#include "wayland/buffer.hpp"
#include "wayland/region.hpp"
#include "wayland/output.hpp"


namespace bicycle_engine::wayland::surface {
    
    /**
     * Wrapper aroung wayland wl_surface api.
     * NOTE: For now it is mix of wl_surface, xdg_surface and xdg_toplevel surface.
     *
     * It will handle frame done event constantly by itself.
     *
     */
    class WLSurface {
    public:
        using native_t = struct wl_surface;
        using native_ptr_t = native_t *;
        using surface_uptr = std::unique_ptr<struct wl_surface, decltype(&wl_surface_destroy)>;
        using surface_listener_t = struct wl_surface_listener;
        using surface_cb_t = std::function<void(Output::native_ptr_t)>;
        using preferred_buffer_scale_cb_t = std::function<void(int)>;
        using preferred_buffer_transform_cb_t = std::function<void(enum wl_output_transform)>;

        WLSurface(struct wl_compositor* compositor, struct xdg_wm_base* xdg_wm_base);
        WLSurface(const WLSurface&) = delete;
        WLSurface& operator=(const WLSurface&) = delete;
        // Requests

        // Set the surface content
        void attach(Buffer& buf);

        // Request frame throttling hint
        void frame(FrameCallback::user_frame_cb cb);

        // Set opaque region
        void set_opaque_region(Region& reg);

        // Set input region
        void set_input_region(Region& reg);

        // Commit changes to surface
        void commit();

        /**
         *  Mark part of the surface damaged using buffer coordinates
         *
         * 	wl_surface.commit assigns pending damage as the current damage, and clears pending damage.
         * 	The server will clear the current damage as it repaints the surface.
         *
         * 	@param x buffer-local x coordinate
         * 	@param y buffer-local y coordinate
         * 	@param width width of damage rectangle
         * 	@param height height of damage rectangle
         */
        void damage_buffer(int x, int y, int width, int height);

        /**
         *  Set the surface contents offset
         *
         *  @param x surface-local x coordinate
         *  @param y surface-local y coordinate
         */
        void offset(int x, int y);

        // Events
        void set_enter_cb(surface_cb_t cb);

        void set_leave_cb(surface_cb_t cb);

        void set_preferred_buffer_scale(preferred_buffer_scale_cb_t cb);

        void set_preferred_buffer_transform(preferred_buffer_transform_cb_t cb);

        ~WLSurface();

        friend class FrameCallback;
    private:
        // xdg_surface events
        static void xdg_surface_configure(void *data,
                                          struct xdg_surface *xdg_surface,
                                          uint32_t serial);
        // wl_surface events
        static void enter_general_cb(void* data,
                                     native_ptr_t surface,
                                     Output::native_ptr_t output);
        static void leave_general_cb(void* data,
                                     native_ptr_t surface,
                                     Output::native_ptr_t output);
        static void scale_general_cb(void *data,
                                     native_ptr_t wl_surface,
                                     int32_t factor);
        static void transform_general_cb(void *data,
                                         native_ptr_t surface,
                                         uint32_t transform);

        std::optional<surface_cb_t> enter_cb;
        std::optional<surface_cb_t> leave_cb;
        std::optional<preferred_buffer_scale_cb_t> scale_cb;
        std::optional<preferred_buffer_transform_cb_t> transform_cb;
        // Wayland surface
        surface_uptr surface;
        // XDG surface 
        struct xdg_surface* xdg_surface;
        // XDG top level surface
        struct xdg_toplevel* xdg_toplevel;

        std::unique_ptr<surface_listener_t> surface_listener;
        // Listeners
        // XDG listener
        struct xdg_surface_listener xdg_surface_listener = {
            .configure = WLSurface::xdg_surface_configure
        };
    };
}

#endif /* BC_WAYLAND_SURFACE_H */
