#include "wayland/display.hpp"

namespace bicycle_engine::wayland {
    Display::Display() : Display(nullptr) {}

    Display::Display(const std::string& name) : Display(name.c_str()) {}

    Display::Display(const char* display_name) : display_(wl_display_connect(display_name), wl_display_disconnect) {
    }

    Display::wl_registry_uptr Display::get_registry() { return wl_registry_uptr(wl_display_get_registry(display_.get()), wl_registry_destroy); }

    int Display::roundtrip() { return wl_display_roundtrip(display_.get()); }
    int Display::dispatch() { return wl_display_dispatch(display_.get()); }
}
