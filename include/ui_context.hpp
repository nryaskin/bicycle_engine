#ifndef BE_UI_CONTEXT_H
#define BE_UI_CONTEXT_H

#include <memory>
#include <vector>

#include <boost/asio.hpp>
#include <boost/asio/posix/stream_descriptor.hpp>

#include "wayland/display.hpp"
#include "wayland/registry.hpp"
#include "wayland/output.hpp"
#include "output.hpp"
#include "graphic_device.hpp"

namespace bicycle_engine {
    class UIContext {
    public:
        using output_cb_t = std::function<void(OutputDescription&)>;
        using graphics_cb_t  = std::function<void(GraphicsDevice)>;
        UIContext(boost::asio::io_context& io);

        void read_outputs(output_cb_t output_cb);
        void get_graphics(uint32_t width, uint32_t height, graphics_cb_t graphics_cb);
    private:

        void prepare_registry_callback_map();

        void prepare_loop();

        void wait_handler(const boost::system::error_code& error);
        void output_received(wayland::Registry& registry, uint32_t name, const wayland::Registry::wl_interface_t *interface, uint32_t version);

        wayland::Display display;
        boost::asio::io_context& io_;
        boost::asio::posix::stream_descriptor stream_;
        std::shared_ptr<wayland::Registry>  registry;
        wayland::Registry::user_callback_map_t callback_map;
        std::vector<OutputDescription> outputs;
        std::vector<std::shared_ptr<wayland::Output>> w_outputs;
        output_cb_t output_cb;
        graphics_cb_t graphics_cb;
    };
}

#endif /* BE_UI_CONTEXT_H */
