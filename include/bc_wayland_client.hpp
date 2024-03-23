#ifndef BICYCLE_ENGINE_WAYLAND_CLIENT_H
#define BICYCLE_ENGINE_WAYLAND_CLIENT_H
#include <memory>
#include <unordered_map>

#include "memory/bc-shm.hpp"
#include "memory/wayland/bc_wayland_shm_pool.hpp"
#include "wayland/display.hpp"
#include "wayland/surface.hpp"
#include "wayland/interface/bc_wayland_shm.hpp"
#include "wayland/interface/global_object_cache.hpp"

namespace bicycle_engine::wayland {

    using wl_registry_t = struct wl_registry;
    namespace wlif = wayland::interface;

    class FailedToConnectWaylandException : public std::runtime_error {
    public:
        FailedToConnectWaylandException() : std::runtime_error::runtime_error("Failed to connect to Wayland display") {}
    };

    class FailedToGetRegistryException : public std::runtime_error {
    public:
        FailedToGetRegistryException() : std::runtime_error::runtime_error("Failed to Wayland registry") {}
    };

    class WaylandClient {
    public:
        WaylandClient();
        surface::WLSurface create_toplevel_surface();
        memory::SharedMemoryPool create_shared_memory_pool(::bicycle_engine::memory::SharedMemory& shared_mem);
        std::weak_ptr<wlif::WLShm> get_shm();
        void roundtrip();
        void dispatch();
        ~WaylandClient();

    private:

        static void registry_global(void *data,
                                    wl_registry_t *registry,
                                    uint32_t name,
                                    const char *interface,
                                    uint32_t version);

        static void registry_global_remove(void *data,
                                           wl_registry_t *registry,
                                           uint32_t name);

        static void ping(void *data,
                         struct xdg_wm_base *xdg_wm_base,
                         uint32_t serial);

        wayland::Display display;

        // Wayland registry
        wayland::Display::wl_registry_uptr registry;  

        // Compositor used to create surfaces and regions
        struct wl_compositor* compositor;

        // Used for wayland ping pong
        struct xdg_wm_base* xdg_wm_base;

        wlif::GlobalObjectCache go_cache;

        // Global listener
        struct wl_registry_listener m_wc_registry_listener = {
            .global        = WaylandClient::registry_global,
            .global_remove = WaylandClient::registry_global_remove
        };

        // WM base listener
        struct xdg_wm_base_listener xdg_wm_base_listener = {
            .ping = WaylandClient::ping,
        };
    };

}
#endif /* BICYCLE_ENGINE_WAYLAND_CLIENT_H */
