#include "wayland/display.hpp"

namespace bicycle_engine::wayland {
    Display::Display() : Display(nullptr) {}

    Display::Display(const std::string& name) : Display(name.c_str()) {}

    Display::Display(const char* display_name) : display_(wl_display_connect(display_name), wl_display_disconnect) {
    }

    std::shared_ptr<Registry> Display::get_registry(std::map<std::string, Registry::user_registry_cb_t> cbs) {
        auto registry = Registry::create(wl_display_get_registry(display_.get()), cbs);
        roundtrip();
        return registry;
    }

    int Display::roundtrip() { return wl_display_roundtrip(display_.get()); }
    int Display::dispatch() { return wl_display_dispatch(display_.get()); }
}
