#include "memory/bc_wayland_shm.hpp"
#include "memory/bc_error.hpp"
#include <format>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string>
#include <filesystem>

namespace {
    constexpr std::string WAYLAND_SHM_PREFIX = "/BC-Wayland";
}

namespace bicycle_engine::wayland::memory::wrapper {
namespace error = bicycle_engine::wayland::memory::error;

void* map_data(int fd, size_t size) {
    auto data = mmap(nullptr, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (data == MAP_FAILED) {
        throw error::ShmMmapException(fd, size, PROT_READ | PROT_WRITE, MAP_SHARED, errno);
    }
    return data;
}

void munmap_data(void *data, size_t size) {
    if (munmap(data, size) == -1) {
        // TODO: Think of how to better log what exactly data are we unmapping
        throw error::ShmMunmapException(size, errno);
    }
}

void ftruncate_data(int fd, size_t size) {
    if (ftruncate(fd, size) == -1) {
        throw error::ShmTruncException(fd, size, errno);
    }
}
}

namespace bicycle_engine::wayland::memory {
// Initialize it here, later think if it is good decision.
uint32_t SharedMemory::shared_file_idx = 0;

std::string SharedMemory::generate_shm_file_name(uint32_t idx) {
    return std::move(std::format("{}-{}", ::WAYLAND_SHM_PREFIX, idx));
}

SharedMemory::SharedMemory(size_t size): fd(-1), size(size), data(nullptr) {
    name = SharedMemory::generate_shm_file_name(SharedMemory::shared_file_idx++);
    fd =  shm_open(name.c_str(), O_RDWR | O_CREAT | O_EXCL | O_TRUNC, 0600);

    if (fd < 0) {
        std::filesystem::path p(name);
        // Pass name to this exception
        throw error::ShmOpenException(p, errno);
    }

    if (size) {
        wrapper::ftruncate_data(fd, size);
        wrapper::map_data(fd, size);
    }
}

// TODO: Update move constructor
SharedMemory::SharedMemory(SharedMemory&& other) : fd(-1), name{""} {
    std::swap(fd, other.fd);
    std::swap(name, other.name);
}


SharedMemory& SharedMemory::operator=(SharedMemory&& other) {
    if (this != &other) {
        fd = other.fd;
        name = other.name;

        other.fd = -1;
        other.name = "";
    }
    return *this;
}

void SharedMemory::resize(size_t size_) {
    if (size == size_) {
        return;
    }
    wrapper::munmap_data(data, size);
    size = size_;
    wrapper::ftruncate_data(fd, size); 
    data = (size) ? wrapper::map_data(fd, size) : nullptr;
}

SharedMemory::~SharedMemory() {
    if (data) {
        // No exceptions here so leaving simple munmap not wrapper
        munmap(data, size);
        data = nullptr;
        size = 0;
    }
    if (fd >= 0) {
        close(fd);
        fd = -1;
    }
    if (name.size()) {
        shm_unlink(name.c_str());
        name =  "";
    }
}
}

