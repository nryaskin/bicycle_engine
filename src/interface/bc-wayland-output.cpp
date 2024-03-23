#include "wayland/interface/bc_wayland_output.hpp"

namespace bicycle_engine::wayland::interface {
    WLOutputService::WLOutputService() : wl_output_(nullptr) {}

    WLOutputService::WLOutputService(struct wl_registry * registry, uint32_t name, const char *interface, uint32_t version) {
        wl_output_.reset(static_cast<struct wl_output*>(wl_registry_bind(registry,
                                              name,
                                              &wl_output_interface,
                                              4)));
        wl_output_add_listener(wl_output_.get(), wl_output_listener.get(), this);
    }

    WLOutputService::WLOutputService(WLOutputService&& other) {
        WLOutputService::swap(*this, other); 
    }

    WLOutputService& WLOutputService::operator=(WLOutputService&& other) {
        WLOutputService::swap(*this, other); 
        return *this;
    }

    void WLOutputService::swap(WLOutputService& lhs, WLOutputService& rhs) {
        std::swap(lhs.wl_output_, rhs.wl_output_);
        std::swap(lhs.wl_output_listener, rhs.wl_output_listener);
    }

    void WLOutputService::geometry_handler(void *data,
                                    struct wl_output *wl_output,
                                    int32_t x,
                                    int32_t y,
                                    int32_t physical_width,
                                    int32_t physical_height,
                                    int32_t subpixel,
                                    const char *make,
                                    const char *model,
                                    int32_t transform) {
        WLOutputService* output = static_cast<WLOutputService *>(data); 
        output->geometry_.x = x;
        output->geometry_.y = y;
        output->geometry_.physical_width = physical_width;
        output->geometry_.physical_height = physical_height;
        output->geometry_.subpixel = subpixel;
        output->geometry_.make = make;
        output->geometry_.model = model;
        output->geometry_.transform = transform;
        if (output->geo_cb) {

        }
    }

    void WLOutputService::mode_handler(void *data,
                                struct wl_output *wl_output,
                                uint32_t flags,
                                int32_t width,
                                int32_t height,
                                int32_t refresh) {
        WLOutputService* output = static_cast<WLOutputService *>(data); 
        output->mode_.flags = flags;
        output->mode_.height = height;
        output->mode_.width = width;
        output->mode_.refresh = refresh;
    }

    void WLOutputService::done_handler(void *data, struct wl_output *wl_output) {
        // TODO: Do something when we receive this for the first time
    }

    void WLOutputService::scale_handler(void *data,
                                 struct wl_output *wl_output,
                                 int32_t factor) {
        WLOutputService* output = static_cast<WLOutputService *>(data); 
        output->scale_ = factor;
    }

    void WLOutputService::name_handler(void *data, struct wl_output *wl_output, const char *name) {
        WLOutputService* output = static_cast<WLOutputService *>(data); 
        output->name_ = name;
    }

    void WLOutputService::description_handler(void *data, struct wl_output *wl_output, const char *description) {
        WLOutputService* output = static_cast<WLOutputService *>(data); 
        output->description_ = description;
    }
}
