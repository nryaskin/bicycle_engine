#ifndef BC_WAYLAND_OUTPUT_H
#define BC_WAYLAND_OUTPUT_H
#include <memory>
#include <string>
#include <functional>

#include "wayland_client_header.h"

#include "wayland/interface.hpp"

namespace bicycle_engine::wayland {

    // Compositor output region.
    class Output : public interface::Interface, std::enable_shared_from_this<Output> {
    public:
        using native_t = struct wl_output;
        using native_ptr_t = native_t *;
        using native_deleter_t = decltype(&wl_output_destroy);
        using native_listener_t = struct wl_output_listener;

        using geometry_cb_t = std::function<void(int32_t,
                                                 int32_t,
                                                 int32_t,
                                                 int32_t,
                                                 int32_t,
                                                 const std::string&,
                                                 const std::string&,
                                                 int32_t)>;
        using mode_cb_t = std::function<void(uint32_t,
                                             int32_t,
                                             int32_t,
                                             int32_t)>;
        using done_cb_t = std::function<void()>;
        using scale_cb_t = std::function<void(int32_t)>;
        using name_cb_t = std::function<void(const std::string&)>;
        using description_cb_t = std::function<void(const std::string&)>;

        static constexpr native_deleter_t native_deleter = wl_output_destroy;

    private:
        Output();
        Output(native_ptr_t ptr);
        Output(std::unique_ptr<native_t, native_deleter_t>&& uptr,
               geometry_cb_t    geom_cb,
               mode_cb_t        mode_cb,
               done_cb_t        done_cb,
               scale_cb_t       scale_cb,
               name_cb_t        name_cb,
               description_cb_t descr_cb);

    public:
        static std::shared_ptr<Output> create(std::unique_ptr<native_t, native_deleter_t>&& uptr,
                                              geometry_cb_t    geom_cb,
                                              mode_cb_t        mode_cb,
                                              done_cb_t        done_cb,
                                              scale_cb_t       scale_cb,
                                              name_cb_t        name_cb,
                                              description_cb_t descr_cb);

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
        native_listener_t output_listener = {
            .geometry = Output::geometry_cb,
            .mode = Output::mode_cb,
            .done = Output::done_cb,
            .scale = Output::scale_cb,
            .name = Output::name_cb,
            .description = Output::description_cb,
        };

        geometry_cb_t       user_geometry_cb;
        mode_cb_t           user_mode_cb;
        done_cb_t           user_done_cb;
        scale_cb_t          user_scale_cb;
        name_cb_t           user_name_cb;
        description_cb_t    user_description_cb;
    };
}

#endif /* BC_WAYLAND_OUTPUT_H */
