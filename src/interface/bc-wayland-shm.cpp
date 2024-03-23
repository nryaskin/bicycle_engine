#include "wayland/interface/bc_wayland_shm.hpp"
#include <cstring>
#include "logger/logger.hpp"

namespace bicycle_engine::wayland::interface {
    namespace belog = bicycle_engine::logger;

    WLShm::WLShm(struct wl_registry * registry, uint32_t name, const char *interface, uint32_t version) {
        if (std::strcmp(interface, wl_shm_interface.name) == 0) {
            // TODO: This part can be moved to Base class with template using it's interface_native_p.
            wl_shm = static_cast<interface_native_p>(wl_registry_bind(registry, name, &wl_shm_interface, 1));
            wl_shm_add_listener(wl_shm, &wl_shm_listener, this);
        }
    }

    WLShm::WLShm(WLShm&& other) {

    }

    WLShm& WLShm::operator=(WLShm&&) {

        return *this;
    }

    void WLShm::format(void *data, struct wl_shm *wl_shm, uint32_t format) {
        belog::Logger("Received format: {:#X}", format);
        WLShm* wshm = static_cast<WLShm*>(data);
        wshm->data_formats.insert(static_cast<wl_shm_format>(format));
    }

    WLShm::interface_native_p WLShm::get_native() {
        return this->wl_shm;
    }
}
