#include <format>

#include "wayland/interface/bc_wayland_interface.hpp"

namespace bicycle_engine::wayland::interface {

    WLInterface::WLInterface() {}

    WLInterface::WLInterface(uint32_t name, const char *interface, uint32_t version) : name_(name), interface_(interface), version_(version) {}

    void WLInterface::swap(WLInterface& lhs, WLInterface& rhs) {
        std::swap(lhs.interface_, rhs.interface_);
        std::swap(lhs.name_, rhs.name_);
        std::swap(lhs.version_, rhs.version_);
    }

    WLInterface::WLInterface(WLInterface&& other) {
        swap(*this, other);
    }

    WLInterface& WLInterface::operator=(WLInterface&& other) {
        swap(*this, other); 
        return *this;
    }

    std::string WLInterface::print() const {
        std::string info = std::format("interface: {}, version {}, name: {}, update_time: {}", interface_, version_, name_, std::asctime(std::localtime(&update_time_)));
        return std::move(info);
    }
}
