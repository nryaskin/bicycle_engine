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

        /**
         * Get a display context's file descriptor
         *
         * @return Display object file descriptor
         */
        int get_fd();

        /**
         * Prepare to read events from the display's file descriptor.
         *
         * @return true on success or false if event queue was not empty
         */
        bool prepare_read();

        /**
         * Dispatch default queue events without reading
         * from the display fd. 
         *
         * @return The number of dispatched events or -1 on failure
         */
        int dispatch_pending();

        /**
         * Cancel read intention on display's fd.
         */
        void cancel_read();

        //TODO: Maybe make it return some error instead of errno.
        //      Need to explore available errors to prepare set of errors to return.
        /**
         * Read events from display file descriptor. 
         *
         * @return true on success or false on error. In case of error errno will be set accordingly
         */
        bool read_events();

        /**
         * Send all buffered requests on the display to the server.
         *
         * @return The number of bytes sent on success or -1 on failure
         */
        int flush();

        // Block until all pending events are processed by server.
        //
        // @return number of dispatched events on success or -1 on failure
        int roundtrip();

        // Process incoming events
        //
        // @return number of dispatched events on succes or -1 on failure
        int dispatch();

        // Get wl_registry object.
        std::shared_ptr<Registry> get_registry(std::map<std::string, Registry::user_registry_cb_t> cbs);
    private:
        Display(const char*);
        std::unique_ptr<struct wl_display, decltype(&wl_display_disconnect)> display_;
    };
}

#endif /* BE_WAYLAND_DISPLAY_HPP */
