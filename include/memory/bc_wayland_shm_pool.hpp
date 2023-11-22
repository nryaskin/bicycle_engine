#ifndef BC_WAYLAND_SHM_POOL_H
#define BC_WAYLAND_SHM_POOL_H
#include "memory/bc_wayland_shm.hpp"
#include <memory>
#include <wayland-client-header.h>

namespace bicycle_engine::wayland::memory {
    class SharedMemoryPool {
    public:
        using WlShmPool = struct wl_shm_pool; 
        using WlShmPoolDeleter = decltype(&wl_shm_pool_destroy);
        using SharedBuffer = struct wl_buffer;
        using SharedBufferPtr = SharedBuffer *;
        using WlShmPtr = struct wl_shm*;

        SharedMemoryPool(WlShmPtr, SharedMemory&);
        SharedBufferPtr create_buffer(int32_t offset, int32_t width, int32_t height, int32_t stride, uint32_t format);
    private:
        std::unique_ptr<WlShmPool, WlShmPoolDeleter> wl_shm_pool;
    };
}
#endif /* BC_WAYLAND_SHM_POOL_H */
