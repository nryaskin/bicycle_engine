#include <iostream>
#include <format>
#include "wayland_client_header.h"
#include <poll.h>
#include <unistd.h>

void global_cb(void *data,
               struct wl_registry *wl_registry,
               uint32_t name,
               const char *interface,
               uint32_t version) {
}

void global_remove_cb(void *data,
                      struct wl_registry *wl_registry,
                      uint32_t name) {
}

int main() {
    std::cout << "Connect to display!" << "\n";
    wl_display* display = wl_display_connect(NULL);
    std::cout << "Get registry!" << "\n";
    wl_registry * registry = wl_display_get_registry(display);
    struct wl_registry_listener listener = {
        .global = global_cb,
        .global_remove = global_remove_cb,
    };

    wl_registry_add_listener(registry, &listener, NULL);
#ifdef EVENT_LOOP
    while (true) {
        int fd = wl_display_get_fd(display);

        while(wl_display_prepare_read(display) != 0) {
            wl_display_dispatch_pending(display);
        }
        /** Send all buffered requests on the display to the server
         *
         * \param display The display context object
         * \return The number of bytes sent on success or -1 on failure
         *
         * Send all buffered data on the client side to the server. Clients should
         * always call this function before blocking on input from the display fd.
         * On success, the number of bytes sent to the server is returned. On
         * failure, this function returns -1 and errno is set appropriately.
         *
         * wl_display_flush() never blocks.  It will write as much data as
         * possible, but if all data could not be written, errno will be set
         * to EAGAIN and -1 returned.  In that case, use poll on the display
         * file descriptor to wait for it to become writable again.
         *
         * \memberof wl_display
         */
        wl_display_flush(display);

        pollfd fds[1] = {
            {
                fd, POLLIN, 0
            }
        };
        nfds_t nfds = 1;

        auto ready = poll(fds, nfds, -1);
        if (ready == -1) {
            exit(1);
        }
        wl_display_read_events(display);
        wl_display_dispatch_pending(display);
        break;
    }
#endif /* EVENT_LOOP */
#ifdef ROUNDTRIP
    wl_display_roundtrip(display);
#endif /* ROUNDTRIP */

    return 0;
}
