#include <iostream>
#include <concepts>
#include <cassert>

#include "wayland/display.hpp"
#include "wayland/registry.hpp"
#include "wayland/output.hpp"


namespace bew = bicycle_engine::wayland;


int main() {
    try {
        bew::Display display;
        std::map<std::string, bew::Registry::user_registry_cb_t> m;
        std::unique_ptr<bew::Output> output(nullptr);
        m[wl_output_interface.name] = [&output](bew::Registry& registry, uint32_t name, const bew::Registry::wl_interface_t *interface, uint32_t version) {
            auto&& output_ptr = registry.bind<bew::Output::native_t, bew::Output::native_deleter_t>(name, interface, version, bew::Output::native_deleter);
            output.reset(new bew::Output(std::move(output_ptr), [](std::string& descr) { std::cout << "Description: " << descr << "\n"; }));
        };
        bew::Registry registry = display.get_registry(m);
        display.dispatch();
    }
    catch (...) {
        throw;
    }
    return 0;
}
