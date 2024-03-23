#ifndef BC_WAYLAND_OUTPUT_H
#define BC_WAYLAND_OUTPUT_H
#include "wayland_client_header.h"

namespace bicycle_engine::wayland {
    using Output = struct wl_output;
    using output_native_t = Output;
    using output_native_ptr_t = output_native_t *;
}

#endif /* BC_WAYLAND_OUTPUT_H */
