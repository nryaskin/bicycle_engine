#include <iostream>
#include <concepts>
#include <cassert>
#include "format"

#include "wayland/display.hpp"
#include "wayland/registry.hpp"
#include "wayland/output.hpp"


namespace bew = bicycle_engine::wayland;

struct output_description_t {
    int32_t     x;
    int32_t     y;
    int32_t     physical_width;
    int32_t     physical_height;
    int32_t     subpixel;
    std::string make;
    std::string model;
    int32_t     transform;
    uint32_t    flags;
    int32_t     width;
    int32_t     height;
    int32_t     refresh;
    int32_t     factor;
    std::string name;
    std::string description;

    friend std::ostream& operator<<(std::ostream& os, const output_description_t& od);

    std::string subpixel_description() const {
        switch (subpixel) {
            case WL_OUTPUT_SUBPIXEL_UNKNOWN:
                return "Unknown";
            case WL_OUTPUT_SUBPIXEL_NONE:
                return "None";
            case WL_OUTPUT_SUBPIXEL_HORIZONTAL_RGB:
                return "Horizontal RGB";
            case WL_OUTPUT_SUBPIXEL_HORIZONTAL_BGR:
                return "Horizontal BGR";
            case WL_OUTPUT_SUBPIXEL_VERTICAL_RGB:
                return "Vertical RGB";
            case WL_OUTPUT_SUBPIXEL_VERTICAL_BGR:
                return "Vertical BGR";
            default:
                return "Unhandled format";
        }
    }

    std::string transform_description() const {
        switch (transform) {
            case WL_OUTPUT_TRANSFORM_NORMAL:
                return "no transform";
            case WL_OUTPUT_TRANSFORM_90:
                return "1-90 degrees counter-clockwise";
            case WL_OUTPUT_TRANSFORM_180:
                return "2-180 degrees counter-clockwise";
            case WL_OUTPUT_TRANSFORM_270:
                return "3-270 degrees counter-clockwise";
            case WL_OUTPUT_TRANSFORM_FLIPPED:
                return "4 - 180 degree flip around a vertical axis";
            case WL_OUTPUT_TRANSFORM_FLIPPED_90:
                return "5 - flip and rotate 90 degrees counter-clockwise";
            case WL_OUTPUT_TRANSFORM_FLIPPED_180:
                return "6 - flip and rotate 180 degrees counter-clockwise";
            case WL_OUTPUT_TRANSFORM_FLIPPED_270:
                return "7 - flip and rotate 270 degrees counter-clockwise";
            default:
                return "Unhandled transform";
        }
    }

    std::string mode_description() const {
        switch (flags) {
            case WL_OUTPUT_MODE_CURRENT:
                return "current";
            case WL_OUTPUT_MODE_PREFERRED:
                return "prefferd";
            default:
                return "Unhandled mode";
        }
    }
};

std::ostream& operator<<(std::ostream& os, const output_description_t& od) {
    os << std::format("Manufacturer : {}\n", od.make);
    os << std::format("Model: {}\n", od.model);
    os << std::format("Name: {}\n", od.name);
    os << std::format("Description: {}\n", od.description);
    os << std::format("Position within the global compositor space: [{},{}]\n", od.x, od.y);
    os << std::format("Physical size: {} mm x {} mm\n", od.physical_width, od.physical_height);
    os << std::format("Subpixel: {}\n", od.subpixel_description());
    os << std::format("Transform: {}\n", od.transform_description());
    os << std::format("Resolution: {} x {}, refresh rate: {} mHZ, mode: {}\n", od.width, od.height, od.refresh, od.mode_description());
    os << std::format("Scale: {}\n", od.factor);
    return os;
}

class UIContext {
public:
    UIContext() {
        callback_map[wl_output_interface.name] = [&](bew::Registry& registry, uint32_t name, const bew::Registry::wl_interface_t *interface, uint32_t version) { this->output_received(registry, name, interface, version); };
        registry = display.get_registry(callback_map);
        display.roundtrip();
    }

    void run() {
        if (!stop) {
            display.dispatch();
        }
    }

    ~UIContext() {}
private:
    void output_received(bew::Registry& registry, uint32_t name, const bew::Registry::wl_interface_t *interface, uint32_t version) {
            auto&& output_ptr = registry.bind<bew::Output::native_t, bew::Output::native_deleter_t>(name, interface, version, bew::Output::native_deleter);
            output = bew::Output::create(std::move(output_ptr),
                                [this](int32_t x, int32_t y, int32_t physical_width, int32_t physical_height, int32_t subpixel, const std::string &make, const std::string &model, int32_t transform) { this->geometry_cb(x, y, physical_width, physical_height, subpixel, make, model, transform); },
                                [this](uint32_t flags, int32_t width, int32_t height, int32_t refresh) { this->mode_cb(flags, width, height, refresh); },
                                [this]() { this->done_cb(); },
                                [this](int32_t factor) { this->scale_cb(factor); },
                                [this](const std::string& name) { this->name_cb(name); },
                                [this](const std::string& description) { this->description_cb(description); }
                    );

    }

    void geometry_cb(int32_t x,
                     int32_t y,
                     int32_t physical_width,
                     int32_t physical_height,
                     int32_t subpixel,
                     const std::string& make,
                     const std::string& model,
                     int32_t transform) {
        output_description.x = x;
        output_description.y = y;
        output_description.physical_width = physical_width;
        output_description.physical_height = physical_height;
        output_description.subpixel = subpixel;
        output_description.make = make;
        output_description.model = model;
        output_description.transform = transform;
    }

    void mode_cb(uint32_t flags,
                 int32_t  width,
                 int32_t  height,
                 int32_t  refresh) {
        output_description.flags = flags;
        output_description.width = width;
        output_description.height = height;
        output_description.refresh = refresh;
    }

    void done_cb() {
        stop = true;
        std::cout << output_description;
    }

    void scale_cb(int32_t factor) {
        output_description.factor = factor;
    }

    void name_cb(const std::string& name) {
        output_description.name = name;
    }

    void description_cb(const std::string& description) {
        output_description.description = description;
    }

    output_description_t            output_description;
    bew::Display                    display;
    std::shared_ptr<bew::Registry>  registry;
    std::shared_ptr<bew::Output>    output;
    std::map<std::string, bew::Registry::user_registry_cb_t> callback_map;
    bool stop = false;
};

int main() {
    try {
        UIContext ui_ctx;
        ui_ctx.run();
    }
    catch (...) {
        throw;
    }
    return 0;
}
