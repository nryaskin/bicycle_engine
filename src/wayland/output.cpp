#include "wayland/output.hpp"

#include <cassert>

namespace bicycle_engine::wayland {

    Output::Output() : output(nullptr, wl_output_destroy) {
        output_listener->description = Output::description_cb;
        output_listener->done = Output::done_cb;
        output_listener->geometry = Output::geometry_cb;
        output_listener->mode = Output::mode_cb;
        output_listener->name = Output::name_cb;
        output_listener->scale = Output::scale_cb;
    }

    Output::Output(native_ptr_t output_ptr) : Output()  {
        output.reset(output_ptr);
        wl_output_add_listener(output.get(), output_listener.get(), this);
    }

    Output::Output(std::unique_ptr<native_t, native_deleter_t>&& uptr, description_cb_t descr_cb) : Output()  {
        output.swap(uptr);
        user_description_cb = descr_cb;
        wl_output_add_listener(output.get(), output_listener.get(), this);
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

    }

	void Output::mode_cb(void *data,
                         native_ptr_t wl_output,
                         uint32_t flags,
                         int32_t width,
                         int32_t height,
                         int32_t refresh) {

    }

	void Output::done_cb(void *data, native_ptr_t wl_output) {

    }

	void Output::scale_cb(void *data, native_ptr_t wl_output, int32_t factor) {

    }

	void Output::name_cb(void *data, native_ptr_t wl_output, const char *name) {

    }

	void Output::description_cb(void *data, native_ptr_t wl_output, const char *description) {
        auto output = static_cast<Output*>(data);
        assert(output->output.get() == wl_output);
        std::string descr(description);
        output->user_description_cb(descr);
    }

    void Output::remove(uint32_t name) {
        output.reset(nullptr);
        output_listener.reset(nullptr);
    }

    Output::~Output() {

    }
}
