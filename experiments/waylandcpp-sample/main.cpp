#include <variant>
#include <map>
#include <cassert>
#include <source_location>
#include <iostream>
#include <format>

#include "waylandcpp/wire/socket.hpp"
#include "waylandcpp/wire/buffer_parser.hpp"
#include "waylandcpp/wl_display.hpp"
#include "waylandcpp/wl_registry.hpp"
#include "waylandcpp/wl_callback.hpp"

namespace wi = waylandcpp::interface;
namespace ww = waylandcpp::wire;

namespace waylandcpp::interface {
    void wl_display::error(waylandcpp::wire::wire_object_id_t object_id, waylandcpp::wire::wire_uint_t code, waylandcpp::wire::wire_string_t message) {
        std::cout << std::format("{}> id: {}, code: {}, message: {}", std::source_location::current().function_name(), object_id.value, code.value, message.value) << std::endl; 
    }

    void wl_display::delete_id(waylandcpp::wire::wire_uint_t id) {
        std::cout << std::format("{}> {}", std::source_location::current().function_name(), id.value) << std::endl;
    }

    void wl_registry::global(waylandcpp::wire::wire_uint_t name, waylandcpp::wire::wire_string_t 
    interface, waylandcpp::wire::wire_uint_t version) {
        std::cout << std::format("{}> name: {}, interface: {}, version: {}", std::source_location::current().function_name(), name.value, interface.value, version.value) << std::endl;
    }

    void wl_registry::global_remove(waylandcpp::wire::wire_uint_t name) {
        std::cout << std::format("{}> name: {}", std::source_location::current().function_name(), name.value) << std::endl;
    }

    void wl_callback::done(waylandcpp::wire::wire_uint_t callback_data) {
        std::cout << std::format("{}> callback_data: {}", std::source_location::current().function_name(), callback_data.value) << std::endl;
    }
}

using wl_object_t = std::variant<std::monostate, wi::wl_display, wi::wl_registry, wi::wl_callback>;

int main() {
    ww::WLSocket socket;
    std::map<ww::wire_object_id_t, wl_object_t> objects;
    wi::wl_display display(socket, 1);
    objects.insert({ 1, display});
    display.get_registry(2);
    wi::wl_registry registry(socket, 2);
    objects.insert({ 2, registry});
    display.sync(3);
    wi::wl_callback callback(socket, 3);
    objects.insert({3, callback});


    auto&& buffer = socket.read();

    ww::WireBuffer wb(buffer);
    auto parser = ww::WireBufferParser(wb);
    while (!parser.isEnd()) {
        auto&& ev = parser.event();
        std::visit([e = std::move(ev)](auto &&object) mutable {
            using T = std::decay_t<decltype(object)>;
            if constexpr (std::is_same_v<T, wi::wl_display>) {
                ww::WireBufferParser body(e.buffer);
                switch (e.op_code) {
                    case 0:
                    {
                        auto object_id = body.object_id();
                        auto code = body.uint();
                        auto msg = body.string();
                        object.error(object_id, code, msg);
                    }
                    break;
                    case 1:
                    {
                        auto object_id = body.uint();
                        object.delete_id(object_id);
                    }
                    break;
                    default:
                        assert(false);
                }
            }
            else if constexpr(std::is_same_v<T, wi::wl_registry>) {
                ww::WireBufferParser body(e.buffer);
                switch(e.op_code) {
                    case 0:
                    {
                        auto&& name = body.uint();
                        auto&& interface = body.string();
                        auto&& version = body.uint();
                        object.global(name, interface, version);
                    }
                    break;
                    case 1:
                    {
                        auto&& name = body.uint();
                        object.global_remove(name);
                    }
                    break;
                    default:
                        assert(false);
                }
            }
            else if constexpr(std::is_same_v<T, wi::wl_callback>) {
                if (e.op_code != 0) {
                    assert(false);
                }
                ww::WireBufferParser body(e.buffer);
                object.done(body.uint());
            }
        }, objects[ev.id]);
    }

    return 0;
}
