#include "wayland/display.hpp"

namespace bicycle_engine::wayland {
    Display::Display() : Display(nullptr) {}

    Display::Display(const std::string& name) : Display(name.c_str()) {}

    Display::Display(const char* display_name) : display_(wl_display_connect(display_name), wl_display_disconnect) {
    }

    std::shared_ptr<Registry> Display::get_registry(std::map<std::string, Registry::user_registry_cb_t> cbs) {
        auto registry = Registry::create(wl_display_get_registry(display_.get()), cbs);
        // TODO: Do I need this roundtrip or not?
        roundtrip();
        return registry;
    }

    int Display::get_fd() {
        return wl_display_get_fd(display_.get());
    }

    bool Display::prepare_read() {
        return wl_display_prepare_read(display_.get());
    }

    int Display::dispatch_pending() {
        return wl_display_dispatch_pending(display_.get());
    }

    void Display::cancel_read() {
        wl_display_cancel_read(display_.get());
    }

    bool Display::read_events() {
        return wl_display_read_events(display_.get()) == 0;
    }

    int Display::flush() {
        return wl_display_flush(display_.get());
    }

    int Display::roundtrip() { return wl_display_roundtrip(display_.get()); }
    int Display::dispatch() { return wl_display_dispatch(display_.get()); }
}
