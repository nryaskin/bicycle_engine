# Exploring wayland event loop.

To explore it run wayland client `event_loop_exp` with WAYLAND_DEBUG=1 variable.

Client will output messages that it is sending.

wl_display_roundtrip sends sync request and block until it receives done event.
Maybe I can use this approach to implement blocking api.

There are 3 apps built:
1. Just creates wl_registry which sends request to wayland server on creation if I am correct.
2. Polls after wl_registry creation on fd and receives events from server.
3. Use roundtrip which operates as barier for all events from server which are ordered somehow internally. Sends display send request and receives done event after server sent all events from wl_registry.

Here some good explanation on why functions called in a way they called with event loop enabled. [link|https://www.systutorials.com/docs/linux/man/3-wl_display_prepare_read]

This part explains why we shall call wl_display_dispatch_pending and wl_display_dispatch later which was unintuitive for me:
`Typically, a toolkit will call wl_display_dispatch_pending() before sleeping, to make sure it doesn't block with unhandled events. Upon waking up, it will assume the file descriptor is readable and read events from the fd by calling wl_display_dispatch().`
So maybe it make sense for some cases where we are not polling in loop?
