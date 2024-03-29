#include "wayland/output.hpp"

#include <cassert>

namespace bicycle_engine::wayland {

    Output::Output() : output(nullptr, wl_output_destroy) {
    }

    Output::Output(native_ptr_t output_ptr) : Output()  {
        output.reset(output_ptr);
        wl_output_add_listener(output.get(), &output_listener, this);
    }

    Output::Output(std::unique_ptr<native_t, native_deleter_t>&& uptr,
                   geometry_cb_t    geom_cb,
                   mode_cb_t        mode_cb,
                   done_cb_t        done_cb,
                   scale_cb_t       scale_cb,
                   name_cb_t        name_cb,
                   description_cb_t descr_cb) : Output() {
        output.swap(uptr);
        user_geometry_cb = geom_cb;
        user_mode_cb = mode_cb;
        user_done_cb = done_cb;
        user_scale_cb = scale_cb;
        user_name_cb = name_cb;
        user_description_cb = descr_cb;
        wl_output_add_listener(output.get(), &output_listener, this);
    }

    std::shared_ptr<Output> Output::create(std::unique_ptr<native_t, native_deleter_t>&& uptr,
                                           geometry_cb_t    geom_cb,
                                           mode_cb_t        mode_cb,
                                           done_cb_t        done_cb,
                                           scale_cb_t       scale_cb,
                                           name_cb_t        name_cb,
                                           description_cb_t descr_cb) {
        return std::shared_ptr<Output>(new Output(std::move(uptr), geom_cb, mode_cb, done_cb, scale_cb, name_cb, descr_cb));
    }

	void Output::geometry_cb(void *data,
                             native_ptr_t wl_output,
                             int32_t x,
                             int32_t y,
                             int32_t physical_width,
                             int32_t physical_height,
                             int32_t subpixel,
                             const char *make,
                             const char *model,
                             int32_t transform) {
        auto output = static_cast<Output *>(data);
        assert(output->output.get() == wl_output);
        std::string make_s(make);
        std::string model_s(make);
        output->user_geometry_cb(x, y, physical_width, physical_height, subpixel, make_s, model_s, transform);
    }

	void Output::mode_cb(void *data,
                         native_ptr_t wl_output,
                         uint32_t flags,
                         int32_t width,
                         int32_t height,
                         int32_t refresh) {
        auto output = static_cast<Output *>(data);
        assert(output->output.get() == wl_output);
        output->user_mode_cb(flags, width, height, refresh);
    }

	void Output::done_cb(void *data, native_ptr_t wl_output) {
        auto output = static_cast<Output *>(data);
        assert(output->output.get() == wl_output);
        output->user_done_cb();
    }

	void Output::scale_cb(void *data, native_ptr_t wl_output, int32_t factor) {
        auto output = static_cast<Output *>(data);
        assert(output->output.get() == wl_output);
        output->user_scale_cb(factor);
    }

	void Output::name_cb(void *data, native_ptr_t wl_output, const char *name) {
        auto output = static_cast<Output *>(data);
        assert(output->output.get() == wl_output);
        std::string name_s(name);
        output->user_name_cb(name_s);
    }

	void Output::description_cb(void *data, native_ptr_t wl_output, const char *description) {
        auto output = static_cast<Output*>(data);
        assert(output->output.get() == wl_output);
        std::string descr(description);
        output->user_description_cb(descr);
    }

    void Output::remove(uint32_t name) {
        output.reset(nullptr);
    }

    Output::~Output() {

    }
}
