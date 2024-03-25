#ifndef BE_WAYLAND_INTERFACE_H
#define BE_WAYLAND_INTERFACE_H

#include "wayland_client_header.h"

#include "noncopyable.hpp"

namespace bicycle_engine::wayland::interface {

    class Interface : public noncopyable {
        public:
            virtual void remove(uint32_t name) = 0;
            virtual ~Interface() {}
    };
}
#endif /* BE_WAYLAND_INTERFACE_H */
