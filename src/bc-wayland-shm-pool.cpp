#include "memory/wayland/bc_wayland_shm_pool.hpp"

namespace bicycle_engine::wayland::memory {
    SharedMemoryPool::SharedMemoryPool() : wl_shm_pool(nullptr) {}

    SharedMemoryPool::SharedMemoryPool(bewlif::WLShm& wl_shm,
                                       bemem::SharedMemory& shm)
        : wl_shm_pool(wl_shm_create_pool(wl_shm.get_native(), shm.get_fd(), shm.get_size())) {}



    SharedMemoryPool::SharedMemoryPool(SharedMemoryPool&& other) {
        SharedMemoryPool::swap(*this, other);
    }

    SharedMemoryPool& SharedMemoryPool::operator=(SharedMemoryPool&& other) {
        SharedMemoryPool::swap(*this, other);
        return *this;
    }

    void SharedMemoryPool::swap(SharedMemoryPool& lhs, SharedMemoryPool& rhs) {
        std::swap(lhs.wl_shm_pool, rhs.wl_shm_pool);
    }

    SharedMemoryPool::SharedBufferPtr SharedMemoryPool::create_buffer(int32_t offset, int32_t width, int32_t height, int32_t stride, uint32_t format) {
        return wl_shm_pool_create_buffer(wl_shm_pool.get(), offset, width, height, stride, format);
    }
}
