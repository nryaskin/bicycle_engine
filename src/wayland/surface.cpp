#include "wayland/surface.hpp"

#include "logger/logger.hpp"

namespace bicycle_engine::wayland::surface {
    namespace bclog = bicycle_engine::logger;

    WLSurface::WLSurface(struct wl_compositor* compositor,
                         struct xdg_wm_base* xdg_wm_base)
        : surface(wl_compositor_create_surface(compositor), wl_surface_destroy),
          surface_listener(new surface_listener_t)
    {
        // NOTE: For now creating xdg surface which is toplevel or popup but later think about extensions
        xdg_surface = xdg_wm_base_get_xdg_surface(xdg_wm_base, surface.get());
        xdg_surface_add_listener(xdg_surface, &xdg_surface_listener, this);
        // NOTE: Make it toplevel but later think of variants
        xdg_toplevel = xdg_surface_get_toplevel(xdg_surface);

        surface_listener->enter = WLSurface::enter_general_cb;
        surface_listener->leave = WLSurface::leave_general_cb;
        surface_listener->preferred_buffer_scale = WLSurface::scale_general_cb;
        surface_listener->preferred_buffer_transform = WLSurface::transform_general_cb;
        wl_surface_add_listener(surface.get(), surface_listener.get(), this);
        wl_surface_commit(surface.get());

    }

    void WLSurface::attach(Buffer& buf) {
        wl_surface_attach(surface.get(), &buf, 0, 0);
    }

    void WLSurface::frame(FrameCallback::user_frame_cb cb) {

    }

    void WLSurface::set_opaque_region(Region& reg) {
        wl_surface_set_opaque_region(surface.get(), &reg);
    }

    void WLSurface::set_input_region(Region& reg) {
        wl_surface_set_input_region(surface.get(), &reg);
    }

    void WLSurface::commit() {
        wl_surface_commit(surface.get());
    }

    void WLSurface::damage_buffer(int x, int y, int width, int height) {
        wl_surface_damage_buffer(surface.get(), x, y, width, height); 
    }

    void WLSurface::offset(int x, int y) {
        wl_surface_offset(surface.get(), x, y);
    }

    void WLSurface::set_enter_cb(surface_cb_t cb) {
        enter_cb = cb;
    }

    void WLSurface::set_leave_cb(surface_cb_t cb) {
        leave_cb = cb;
    }

    void WLSurface::set_preferred_buffer_scale(preferred_buffer_scale_cb_t cb) {
        scale_cb = cb; 
    }

    void WLSurface::set_preferred_buffer_transform(preferred_buffer_transform_cb_t cb) {
        transform_cb = cb;
    }

    void WLSurface::xdg_surface_configure(void *data,
                                          struct xdg_surface *xdg_surface,
                                          uint32_t serial) {
        bclog::Logger("serial: {}", serial);
        WLSurface* wlsurface = static_cast<WLSurface *>(data);
        xdg_surface_ack_configure(wlsurface->xdg_surface, serial);

    }

    void WLSurface::enter_general_cb(void* data,
                                     native_ptr_t surface,
                                     Output::native_ptr_t output) {
        auto surface_ = static_cast<WLSurface *>(data);
        if (surface_->enter_cb) {
            surface_->enter_cb.value()(output);
        }
    }

    void WLSurface::leave_general_cb(void* data,
                                     native_ptr_t surface,
                                     Output::native_ptr_t output) {
        auto surface_ = static_cast<WLSurface *>(data);
        if (surface_->leave_cb) {
            surface_->leave_cb.value()(output);
        }
    }

    void WLSurface::scale_general_cb(void *data,
                                     native_ptr_t wl_surface,
                                     int32_t factor) {
        auto surface_ = static_cast<WLSurface *>(data);
        if (surface_->scale_cb) {
            surface_->scale_cb.value()(factor);
        }
    }

    void WLSurface::transform_general_cb(void *data,
                                         native_ptr_t surface,
                                         uint32_t transform) {
        auto surface_ = static_cast<WLSurface *>(data);
        if (surface_->transform_cb) {
            surface_->transform_cb.value()(static_cast<enum wl_output_transform>(transform));
        }
    }

    WLSurface::~WLSurface() {

    }
}
