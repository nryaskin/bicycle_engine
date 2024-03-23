#ifndef BE_FRAME_CALLBACK_H
#define BE_FRAME_CALLBACK_H
#include <memory>

#include "wayland_client_header.h"

namespace bicycle_engine::wayland::surface {
class WLSurface;
class FrameCallback {
public:
    FrameCallback() = default;
    FrameCallback(WLSurface& surface);
    FrameCallback(FrameCallback&&);
    FrameCallback& operator=(FrameCallback&&);
    ~FrameCallback();
private:
    static void frame_done(void *data,
                           struct wl_callback *wl_callback,
                           uint32_t callback_data);
    struct wl_callback* frame_cb = nullptr;
    std::unique_ptr<struct wl_callback_listener> wl_surface_frame_listener;
};
}

#endif /* BE_FRAME_CALLBACK_H */
