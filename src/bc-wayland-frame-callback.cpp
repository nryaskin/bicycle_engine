#include "bc_wayland_frame_callback.hpp"

#include "wayland/surface.hpp"
#include "logger/logger.hpp"

namespace bicycle_engine::wayland::surface {
    namespace bclog = bicycle_engine::logger;

    FrameCallback::FrameCallback(WLSurface& surface, user_frame_cb cb) : uframe_cb(cb), wl_surface_frame_listener(new struct wl_callback_listener) {
        frame_cb = wl_surface_frame(surface.surface.get());
        wl_surface_frame_listener->done = frame_done;
        wl_callback_add_listener(frame_cb, wl_surface_frame_listener.get(), this);
    }

    FrameCallback::FrameCallback(FrameCallback&& other) {
        std::swap(frame_cb, other.frame_cb);
        wl_surface_frame_listener.swap(other.wl_surface_frame_listener);
    }

    FrameCallback& FrameCallback::operator=(FrameCallback&& other) {
        std::swap(frame_cb, other.frame_cb);
        wl_surface_frame_listener.swap(other.wl_surface_frame_listener);
        return *this;
    }

    FrameCallback::~FrameCallback() {
        wl_callback_destroy(frame_cb);
    }

    void FrameCallback::frame_done(void *data,
                                   struct wl_callback *wl_callback,
                                   uint32_t callback_data) {
        bclog::Logger();
        auto fb = static_cast<FrameCallback*>(data);
        fb->uframe_cb(callback_data); 
    }
}
