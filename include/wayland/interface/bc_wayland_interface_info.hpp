#ifndef BICYCLE_ENGINE_WAYLAND_INTERFACE_INFO_H
#define BICYCLE_ENGINE_WAYLAND_INTERFACE_INFO_H
#include "wayland/interface/bc_wayland_interface.hpp"

namespace bicycle_engine::wayland::interface {
    // Note: Created to query interface that are not in use right now.
    class WLInterfaceInfo : public WLInterface {
    public:
        WLInterfaceInfo(uint32_t name, const char *interface, uint32_t version) : WLInterface(name, interface, version) {
        }
    };
}

#endif /* BICYCLE_ENGINE_WAYLAND_INTERFACE_INFO_H */
