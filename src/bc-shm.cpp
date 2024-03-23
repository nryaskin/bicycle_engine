#include "memory/bc-shm.hpp"
#include "memory/bc_error.hpp"
#include "memory/bc-shm-file.hpp"
#include <format>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <string>
#include <filesystem>

namespace {
    constexpr std::string WAYLAND_SHM_PREFIX = "/BC-Wayland";
}

namespace bicycle_engine::memory::wrapper {
    void* map_data(int fd, size_t size) {
        auto data = mmap(nullptr, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
        if (data == MAP_FAILED) {
            throw error::ShmMmapException(fd, size, PROT_READ | PROT_WRITE, MAP_SHARED, errno);
        }
        return data;
    }

    void munmap_data(void *data, size_t size) {
        if (munmap(data, size) == -1) {
            // TODO: Think of how to provide better info about error
            throw error::ShmMunmapException(size, errno);
        }
    }
}

namespace bicycle_engine::memory {
// Initialize it here, later think if it is good decision.
uint32_t SharedMemory::shared_file_idx = 0;

const std::string SharedMemory::generate_shm_file_name() {
    return std::move(std::format("{}-{}", ::WAYLAND_SHM_PREFIX, SharedMemory::shared_file_idx++));
}

SharedMemory::SharedMemory(size_t size): size(size), data(nullptr) {
    auto name = SharedMemory::generate_shm_file_name();
    auto shared_file = SharedFile(name);

    if (size) {
        shared_file.truncate(size);
        data = wrapper::map_data(shared_file.get_fd(), size);
    }
}

SharedMemory::SharedMemory(SharedMemory&& other) : size(0), data(nullptr) {
    std::swap(shared_file, other.shared_file);
    std::swap(size, other.size);
    std::swap(data, other.data);
}

SharedMemory& SharedMemory::operator=(SharedMemory&& other) {
    if (this != &other) {
        std::swap(shared_file, other.shared_file);
        std::swap(size, other.size);
        std::swap(data, other.data);
    }
    return *this;
}

void SharedMemory::resize(size_t size_) {
    if (size == size_) {
        return;
    }
    if (data && size) {
        wrapper::munmap_data(data, size);
    }
    size = size_;

    auto name = SharedMemory::generate_shm_file_name();
    shared_file = SharedFile(name);
    shared_file.truncate(size); 

    data = wrapper::map_data(shared_file.get_fd(), size);
}

SharedMemory::~SharedMemory() {
    if (data && size > 0) {
        // No exceptions here so leaving simple munmap not wrapper
        munmap(data, size);
        data = nullptr;
        size = 0;
    }
}
}

