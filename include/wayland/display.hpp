#ifndef BE_WAYLAND_DISPLAY_HPP
#define BE_WAYLAND_DISPLAY_HPP
#include <memory>

#include "wayland_client_header.h"

#include "noncopyable.hpp"
#include "wayland/registry.hpp"

namespace bicycle_engine::wayland {
    // wl_display object wrapper.
    class Display : public noncopyable {
    public:
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
        Registry get_registry(std::map<std::string, Registry::user_registry_cb_t> cbs);
    private:
        Display(const char*);
        std::unique_ptr<struct wl_display, decltype(&wl_display_disconnect)> display_;
    };
}

#endif /* BE_WAYLAND_DISPLAY_HPP */
