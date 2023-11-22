#include "memory/bc_wayland_shm_pool.hpp"

namespace bicycle_engine::wayland::memory {
    SharedMemoryPool::SharedMemoryPool(WlShmPtr wl_shm, SharedMemory& shm) : wl_shm_pool(std::unique_ptr<WlShmPool, WlShmPoolDeleter>(wl_shm_create_pool(wl_shm, shm.get_fd(), shm.get_size()), wl_shm_pool_destroy)) {}

    SharedMemoryPool::SharedBufferPtr SharedMemoryPool::create_buffer(int32_t offset, int32_t width, int32_t height, int32_t stride, uint32_t format) {
        return wl_shm_pool_create_buffer(wl_shm_pool.get(), offset, width, height, stride, format);
    }
}
