#ifndef BE_FRAME_CALLBACK_H
#define BE_FRAME_CALLBACK_H
#include <memory>
#include <functional>

#include "wayland_client_header.h"

namespace bicycle_engine::wayland::surface {
class WLSurface;
class FrameCallback {
public:
    using user_frame_cb = std::function<void(uint32_t)>;
    FrameCallback() = default;
    FrameCallback(WLSurface& surface, user_frame_cb cb);
    FrameCallback(FrameCallback&&);
    FrameCallback& operator=(FrameCallback&&);
    ~FrameCallback();
private:
    static void frame_done(void *data,
                           struct wl_callback *wl_callback,
                           uint32_t callback_data);
    struct wl_callback* frame_cb = nullptr;
    user_frame_cb uframe_cb;
    std::unique_ptr<struct wl_callback_listener> wl_surface_frame_listener;
};
}

#endif /* BE_FRAME_CALLBACK_H */
