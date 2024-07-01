#include <variant>
#include <unordered_map>
#include <cassert>
#include <source_location>
#include <iostream>
#include <format>

#include "waylandcpp/wire/socket.hpp"
#include "waylandcpp/wire/types.hpp"
#include "waylandcpp/wire/buffer_parser.hpp"
#include "waylandcpp/wl_display.hpp"
#include "waylandcpp/wl_registry.hpp"
#include "waylandcpp/wl_callback.hpp"

namespace wi = waylandcpp::interface;
namespace ww = waylandcpp::wire;

using wl_object_t = std::variant<std::monostate, wi::wl_display, wi::wl_registry, wi::wl_callback>;

class wayland_context_t {
public:
    wayland_context_t() {}

    using native_id = decltype(ww::wire_object_id_t::value);

    template<typename wayland_type>
    wayland_type& create(auto& socket, int id){
        ++max_id;
        wayland_type object(socket, id);
        auto [it, inserted] = objects.emplace(std::make_pair(max_id, object));
        if (inserted) {
            return std::get<wayland_type>(it->second);
        }
        else throw std::runtime_error("Cannot insert new wayland object");
    }

    void dispatch(native_id id, ww::Event& ev) {
        if (auto it = objects.find(id); it != objects.end()) {
            std::visit([&](auto&& wl_obj) {
                using Type = std::decay_t<decltype(wl_obj)>;
                if constexpr (std::is_same_v<std::monostate, Type>) {
                    throw std::runtime_error("We shall not be here!");
                } else {
                    ww::WireBufferParser body(ev.buffer);
                    wl_obj.dispatch(ev.op_code, body);
                }
            }, it->second);
        }
        else throw std::runtime_error("No such object!");
    }

private:
    std::unordered_map<native_id, wl_object_t> objects;
    native_id max_id = 0;
};

int main() {
    wayland_context_t wayctx;
    ww::WLSocket socket;
    auto& display = wayctx.create<wi::wl_display>(socket, 1);
    display.get_registry(2);
    auto& registry = wayctx.create<wi::wl_registry>(socket, 2);
    display.sync(3);
    auto& callback = wayctx.create<wi::wl_callback>(socket, 3);

    auto&& buffer = socket.read();

    ww::WireBuffer wb(buffer);
    auto parser = ww::WireBufferParser(wb);
    while (!parser.isEnd()) {
        auto&& ev = parser.event();
        wayctx.dispatch(ev.id.value, ev);
    }

    return 0;
}
