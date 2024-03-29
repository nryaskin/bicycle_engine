#ifndef BICYCLE_ENGINE_WAYLAND_CLIENT_H
#define BICYCLE_ENGINE_WAYLAND_CLIENT_H

#include <memory>
#include <map>
#include <vector>
#include <functional>

#include "xdg_client_header.h"

#include "noncopyable.hpp"
#include "wayland/interface.hpp"
#include "wayland/interface/global_object_cache.hpp"

namespace bicycle_engine::wayland {

    class FailedToConnectWaylandException : public std::runtime_error {
    public:
        FailedToConnectWaylandException() : std::runtime_error::runtime_error("Failed to connect to Wayland display") {}
    };

    class FailedToGetRegistryException : public std::runtime_error {
    public:
        FailedToGetRegistryException() : std::runtime_error::runtime_error("Failed to Wayland registry") {}
    };

    class Registry : noncopyable, std::enable_shared_from_this<Registry> {
    public:
        using native_t = struct wl_registry;
        using native_ptr_t = native_t *;
        using wl_interface_t = struct wl_interface;
        using registry_uptr = std::unique_ptr<native_t, decltype(&wl_registry_destroy)>;
        using compositor_uptr = std::unique_ptr<struct wl_compositor, decltype(&wl_compositor_destroy)>;
        using xdg_wm_base_uptr = std::unique_ptr<struct xdg_wm_base, decltype(&xdg_wm_base_destroy)>;
        using xdg_wm_base_listener_t = struct xdg_wm_base_listener;
        using registry_listener_t = struct wl_registry_listener;
        using user_registry_cb_t = std::function<void(Registry&, uint32_t, const wl_interface_t *interface, uint32_t)>;

    private:
        Registry(native_ptr_t wl_registry, std::map<std::string, user_registry_cb_t> cbs);

    public:

        static std::shared_ptr<Registry> create(native_ptr_t wl_registry, std::map<std::string, user_registry_cb_t> cbs);

        template<typename T, typename Deleter>
        std::unique_ptr<T, Deleter> bind(uint32_t name,
                                         const struct wl_interface *interface,
                                         uint32_t version,
                                         Deleter deleter) {
            auto ptr = static_cast<T *>(wl_registry_bind(registry.get(), name, interface, version));
            return std::move(std::unique_ptr<T, Deleter>(ptr, deleter));
        }

        ~Registry();

    private:

        static void registry_global(void *data,
                                    native_ptr_t wl_registry,
                                    uint32_t name,
                                    const char *interface,
                                    uint32_t version);

        static void registry_global_remove(void *data,
                                           native_ptr_t wl_registry,
                                           uint32_t name);

        static void ping(void *data,
                         struct xdg_wm_base *xdg_wm_base,
                         uint32_t serial);


        static void swap(registry_uptr& reg1, registry_uptr& reg2);

        // Wayland registry
        registry_uptr registry;

        // Compositor used to create surfaces and regions
        compositor_uptr compositor;

        // Used for wayland ping pong
        xdg_wm_base_uptr xdg_wm_base;

        interface::GlobalObjectCache go_cache;

        // Global listener
        registry_listener_t registry_listener = {
            .global = Registry::registry_global,
            .global_remove = Registry::registry_global_remove,
        };

        // WM base listener
        xdg_wm_base_listener_t xdg_wm_base_listener = {
            .ping = Registry::ping,
        };

        std::map<std::string, user_registry_cb_t> user_registry_cbs;
    };


}
#endif /* BICYCLE_ENGINE_WAYLAND_CLIENT_H */
