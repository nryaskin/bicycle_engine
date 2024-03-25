#ifndef BC_WAYLAND_OUTPUT_H
#define BC_WAYLAND_OUTPUT_H
#include <memory>
#include <string>
#include <functional>

#include "wayland_client_header.h"

#include "wayland/interface.hpp"

namespace bicycle_engine::wayland {

    // Compositor output region.
    class Output : public interface::Interface {
    public:
        using native_t = struct wl_output;
        using native_ptr_t = native_t *;
        using native_deleter_t = decltype(&wl_output_destroy);
        using description_cb_t = std::function<void(std::string&)>;
        static constexpr native_deleter_t native_deleter = wl_output_destroy;

        Output();
        Output(native_ptr_t ptr);
        Output(std::unique_ptr<native_t, native_deleter_t>&& uptr, description_cb_t descr_cb);

        void remove(uint32_t name);
        ~Output();
    private:

        /**
         *  Geometry event internal handler
         *
         */
        static void geometry_cb(void *data,
                                native_ptr_t wl_output,
                                int32_t x,
                                int32_t y,
                                int32_t physical_width,
                                int32_t physical_height,
                                int32_t subpixel,
                                const char *make,
                                const char *model,
                                int32_t transform);
        /**
         *
         */
        static void mode_cb(void *data,
                            native_ptr_t wl_output,
                            uint32_t flags,
                            int32_t width,
                            int32_t height,
                            int32_t refresh);

        /**
         *  
         *
         */
        static void done_cb(void *data, native_ptr_t wl_output);

        static void scale_cb(void *data, native_ptr_t wl_output, int32_t factor);

        static void name_cb(void *data, native_ptr_t wl_output, const char *name);

        static void description_cb(void *data, native_ptr_t wl_output, const char *description);

        std::unique_ptr<native_t, native_deleter_t> output;
        std::unique_ptr<wl_output_listener> output_listener = std::unique_ptr<wl_output_listener>(new wl_output_listener);
        description_cb_t user_description_cb;
    };
}

#endif /* BC_WAYLAND_OUTPUT_H */
