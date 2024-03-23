#ifndef BE_WAYLAND_DISPLAY_HPP
#define BE_WAYLAND_DISPLAY_HPP
#include <memory>

#include "noncopyable.hpp"
#include "wayland_client_header.h"

namespace bicycle_engine::wayland {
    // wl_display object wrapper.
    class Display : public noncopyable {
    public:
        using wl_registry_uptr = std::unique_ptr<struct wl_registry, decltype(&wl_registry_destroy)>;
        Display();
        Display(const std::string& display_name);
        Display(Display&&);
        Display& operator=(Display&&);

        // Block until all pending events are processed by server.
        //
        // @return number of dispatched events on success or -1 on failure
        int roundtrip();

        // Process incoming events
        //
        // @return number of dispatched events on succes or -1 on failure
        int dispatch();

        // Get wl_registry object.
        wl_registry_uptr get_registry();
    private:
        Display(const char*);
        std::unique_ptr<struct wl_display, decltype(&wl_display_disconnect)> display_;
    };
}

#endif /* BE_WAYLAND_DISPLAY_HPP */
