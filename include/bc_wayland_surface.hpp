#ifndef BC_WAYLAND_SURFACE_H
#define BC_WAYLAND_SURFACE_H

#include "xdg_client_header.h"

#include "bc_wayland_frame_callback.hpp"
#include "bc_statistics.hpp"
#include "wayland/display.hpp"
#include "wayland/buffer.hpp"
#include "wayland/region.hpp"


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
        using wl_surface_uptr = std::unique_ptr<struct wl_surface, decltype(&wl_surface_destroy)>;
        WLSurface(struct wl_compositor* compositor, struct xdg_wm_base* xdg_wm_base);
        WLSurface(const WLSurface&) = delete;
        WLSurface& operator=(const WLSurface&) = delete;

        // Set the surface content
        void attach(Buffer& buf);

        // Request frame throttling hint
        void frame(FrameCallback& cb);

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

        ~WLSurface();

        friend class FrameCallback;
    private:
        static void xdg_surface_configure(void *data,
                                          struct xdg_surface *xdg_surface,
                                          uint32_t serial);

        // Wayland surface
        wl_surface_uptr surface;
        // XDG surface 
        struct xdg_surface* xdg_surface;
        // XDG top level surface
        struct xdg_toplevel* xdg_toplevel;

        // Listeners
        // XDG listener
        struct xdg_surface_listener xdg_surface_listener = {
            .configure = WLSurface::xdg_surface_configure
        };
    };
}

#endif /* BC_WAYLAND_SURFACE_H */
