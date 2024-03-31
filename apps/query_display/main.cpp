#include <iostream>
#include <concepts>
#include <cassert>
#include "format"

#include "wayland/display.hpp"
#include "wayland/registry.hpp"
#include "wayland/output.hpp"
#include "output.hpp"


namespace bew = bicycle_engine::wayland;


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
        output_description.geometry_cb(x, y, physical_width, physical_height, subpixel, make, model, transform);
    }

    void mode_cb(uint32_t flags,
                 int32_t  width,
                 int32_t  height,
                 int32_t  refresh) {
        output_description.mode_cb(flags, width, height, refresh);
    }

    void done_cb() {
        stop = true;
        std::cout << output_description;
    }

    void scale_cb(int32_t factor) {
        output_description.scale_cb(factor);
    }

    void name_cb(const std::string& name) {
        output_description.name_cb(name);
    }

    void description_cb(const std::string& description) {
        output_description.description_cb(description);
    }

    bicycle_engine::OutputDescription output_description;
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
