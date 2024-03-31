#include "ui_context.hpp"

#include <iostream>

namespace bicycle_engine {
    UIContext::UIContext(boost::asio::io_context& io) : io_(io), stream_(io_) {
        assert(display.get_fd() >= 0);
        stream_.assign(display.get_fd());
        boost::asio::post(io_, [self = this] { self->prepare_loop();});

        prepare_registry_callback_map();
        registry = display.get_registry(callback_map);
    }

    void UIContext::prepare_loop() {
        while (display.prepare_read() != 0) {
            display.dispatch_pending();
        }
        display.flush();
        stream_.async_wait(boost::asio::posix::stream_descriptor::wait_read, [this](auto err) { this->wait_handler(err); });
    }

    void UIContext::wait_handler(const boost::system::error_code& error) {
        assert(display.read_events());
        display.dispatch_pending();
        boost::asio::post(io_, [self = this] { self->prepare_loop();});
    }

    void UIContext::prepare_registry_callback_map() {
        callback_map[wl_output_interface.name] = [&](wayland::Registry& registry, uint32_t name, const wayland::Registry::wl_interface_t *interface, uint32_t version) { this->output_received(registry, name, interface, version); };
    }

    void UIContext::output_received(wayland::Registry& registry, uint32_t name, const wayland::Registry::wl_interface_t *interface, uint32_t version) {
        auto&& output_ptr = registry.bind<wayland::Output::native_t, wayland::Output::native_deleter_t>(name, interface, version, wayland::Output::native_deleter);
        outputs.push_back(OutputDescription());
        auto& od = outputs.back();
        auto output = wayland::Output::create(std::move(output_ptr),
                            [&od](int32_t x, int32_t y, int32_t physical_width, int32_t physical_height, int32_t subpixel, const std::string &make, const std::string &model, int32_t transform) { od.geometry_cb(x, y, physical_width, physical_height, subpixel, make, model, transform); },
                            [&od](uint32_t flags, int32_t width, int32_t height, int32_t refresh) { od.mode_cb(flags, width, height, refresh); },
                            [this, &od]() { boost::asio::post(this->io_, [this, &od](){ this->output_cb(od); }); },
                            [&od](int32_t factor) { od.scale_cb(factor); },
                            [&od](const std::string& name) { od.name_cb(name); },
                            [&od](const std::string& description) { od.description_cb(description); }
                );
        w_outputs.push_back(std::move(output));
    }

    void UIContext::read_outputs(output_cb_t output_cb) {
        this->output_cb = output_cb; 
    }

    void UIContext::get_graphics(uint32_t width, uint32_t height, graphics_cb_t graphics_cb) {

    }
}
