#include "bc_wayland_surface.hpp"

#include "logger/logger.hpp"

namespace bicycle_engine::wayland::surface {
    namespace bclog = bicycle_engine::logger;

    WLSurface::WLSurface(struct wl_compositor* compositor,
                         struct xdg_wm_base* xdg_wm_base)
        : surface(wl_compositor_create_surface(compositor), wl_surface_destroy)
    {
        // NOTE: For now creating xdg surface which is toplevel or popup but later think about extensions
        xdg_surface = xdg_wm_base_get_xdg_surface(xdg_wm_base, surface.get());
        xdg_surface_add_listener(xdg_surface, &xdg_surface_listener, this);
        // NOTE: Make it toplevel but later think of variants
        xdg_toplevel = xdg_surface_get_toplevel(xdg_surface);
        // QUESTION: Do we need this commit if we commiting xdg in surface_configure event?
        wl_surface_commit(surface.get());

#if 0
        frame_cb = FrameCallback(*this);
#endif
    }

    void WLSurface::xdg_surface_configure(void *data,
                                          struct xdg_surface *xdg_surface,
                                          uint32_t serial) {
        bclog::Logger("serial: {}", serial);
        WLSurface* wlsurface = static_cast<WLSurface*>(data);
        xdg_surface_ack_configure(wlsurface->xdg_surface, serial);

#if 0
        struct wl_buffer* buffer = wlsurface->get_frame();
        wl_surface_attach(wlsurface->surface, buffer, 0, 0);
        wl_surface_commit(wlsurface->surface);
#endif
    }
#if 0
    void WLSurface::frame_done(uint32_t callback_data) {
        bclog::Logger();

        frame_cb = FrameCallback(*this);
        stats.add_frame();
        auto buffer = ws->get_frame();

        wl_buffer_add_listener(buffer, &ws->wl_buffer_listener, ws);
        //NOTE: It is not necessary to attach each time
        wl_surface_attach(ws->surface, buffer, 0, 0);
        // TODO: Compute damaged area so there is not need to repaint full window
        wl_surface_damage_buffer(ws->surface,
                                 0, 0,
                                 std::numeric_limits<int32_t>::max(),
                                 std::numeric_limits<int32_t>::max());
        wl_surface_commit(ws->surface);
        bclog::Logger("Frames: {}", stats.get_statistics());
    }
#endif

    WLSurface::~WLSurface() {

    }
}
