#ifndef BE_MEMORY_WAYLAND_BC_WAYLAND_SHM_H
#define BE_MEMORY_WAYLAND_BC_WAYLAND_SHM_H
#include "wayland/interface/bc_wayland_interface.hpp"
#include "wayland_client_header.h"
#include "noncopyable.hpp"
#include <set>

namespace bicycle_engine::wayland::interface {
    class WLShm : public WLInterface, noncopyable {
    public:
        using interface_native_t = struct wl_shm;
        using interface_native_p = interface_native_t *;
        WLShm();
        WLShm(struct wl_registry * registry, uint32_t name, const char *interface, uint32_t version);
        // Movable
        WLShm(WLShm&&);
        WLShm& operator=(WLShm&&);

        const std::set<wl_shm_format>& available_formats() const { return data_formats; }
        interface_native_p get_native();
    private:
        static void format(void *data, struct wl_shm *wl_shm, uint32_t format);

        // Shared memory registry interface
        interface_native_p wl_shm = nullptr;
        // Supported data formats
        std::set<wl_shm_format> data_formats;
        // Format listener
        struct wl_shm_listener wl_shm_listener = {
            .format = WLShm::format,
        };
    };
}

#endif /* BE_MEMORY_WAYLAND_BC_WAYLAND_SHM_H */
