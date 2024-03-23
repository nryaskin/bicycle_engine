#ifndef BE_MEMORY_WAYLAND_BC_WAYLAND_SHM_POOL_H
#define BE_MEMORY_WAYLAND_BC_WAYLAND_SHM_POOL_H
#include <memory>

#include "wayland_client_header.h"

#include "memory/bc-shm.hpp"
#include "wayland/buffer.hpp"
#include "wayland/interface/bc_wayland_shm.hpp"

namespace bicycle_engine::wayland::memory {
    namespace bemem = bicycle_engine::memory;
    namespace bewlif = bicycle_engine::wayland::interface;

    using WlShmPool = struct wl_shm_pool; 
    using WlShmPtr = struct wl_shm*;

    class SharedMemoryPoolDeleter {
        SharedMemoryPoolDeleter() {}
        SharedMemoryPoolDeleter(const SharedMemoryPoolDeleter&) = delete;
        SharedMemoryPoolDeleter& operator=(SharedMemoryPoolDeleter&) = delete;

        SharedMemoryPoolDeleter(SharedMemoryPoolDeleter&&) = default;
        SharedMemoryPoolDeleter& operator=(SharedMemoryPoolDeleter&&) = default;

        void operator()(WlShmPool* wl_shm_pool) {
            if (wl_shm_pool) {
                wl_shm_pool_destroy(wl_shm_pool);
            }
        }
    };

    class SharedMemoryPool {
    public:

        SharedMemoryPool();
        SharedMemoryPool(bewlif::WLShm&, bemem::SharedMemory&);
        // Non-Copyable
        SharedMemoryPool(const SharedMemoryPool&) = delete;
        SharedMemoryPool& operator=(const SharedMemoryPool&) = delete;
        // Movable
        SharedMemoryPool(SharedMemoryPool&&);
        SharedMemoryPool& operator=(SharedMemoryPool&&);

        static void swap(SharedMemoryPool& lhs, SharedMemoryPool& rhs);
        Buffer* create_buffer(int32_t offset, int32_t width, int32_t height, int32_t stride, uint32_t format);
    private:
        std::unique_ptr<WlShmPool, SharedMemoryPoolDeleter> wl_shm_pool;
    };

}
#endif /* BE_MEMORY_WAYLAND_BC_WAYLAND_SHM_POOL_H */
