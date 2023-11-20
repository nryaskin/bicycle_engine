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

int shm_open_wrapper(const std::string& name) {
    auto fd = shm_open(name.c_str(), O_RDWR | O_CREAT | O_EXCL | O_TRUNC, 0600);

    if (fd < 0) {
        std::filesystem::path p(name);
        throw error::ShmOpenException(p, errno);
    }

    return fd;
}

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

const std::string SharedMemory::generate_shm_file_name() {
    return std::move(std::format("{}-{}", ::WAYLAND_SHM_PREFIX, SharedMemory::shared_file_idx++));
}

SharedMemory::SharedMemory(size_t size): fd(-1), size(size), data(nullptr) {
    auto name = SharedMemory::generate_shm_file_name();
    fd = wrapper::shm_open_wrapper(name);

    if (size) {
        try {
            wrapper::ftruncate_data(fd, size);
            wrapper::map_data(fd, size);
        }
        catch (std::exception& e) {
            if (name.size()) {
                shm_unlink(name.c_str());
            }
            throw e;
        }
    }

    if (name.size()) {
        // NOTE: Calling shm_unlink to prevent dangling file open on system crash,
        // Stack unwinding is not reliable so unlink here and os will remove file itself when there is no mmap to this file.
        // We still have dangling files in case of ftruncate or mmap failure which I will think later on how to address
        shm_unlink(name.c_str());
    }
}

SharedMemory::SharedMemory(SharedMemory&& other) : fd(-1), size(0), data(nullptr) {
    std::swap(fd, other.fd);
    std::swap(size, other.size);
    std::swap(data, other.data);
}


SharedMemory& SharedMemory::operator=(SharedMemory&& other) {
    if (this != &other) {
        fd = other.fd;
        size = other.size;
        data = other.data;

        other.fd = -1;
        other.size = 0;
        other.data = nullptr;
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
    wrapper::ftruncate_data(fd, size); 
    if (size) {
        auto name = SharedMemory::generate_shm_file_name();
        int fd = wrapper::shm_open_wrapper(name);
        // TODO: Wrap in try catch because it can throw it and cause shm_unlink not to happen
        try {
            data = wrapper::map_data(fd, size);
        }
        catch (std::exception& e) {
            if (name.size()) {
                shm_unlink(name.c_str());
            }
            throw e;
        }
        if (name.size()) {
            shm_unlink(name.c_str());
        }
    }
}

SharedMemory::~SharedMemory() {
    if (data && size > 0) {
        // No exceptions here so leaving simple munmap not wrapper
        munmap(data, size);
        data = nullptr;
        size = 0;
    }
    if (fd >= 0) {
        close(fd);
        fd = -1;
    }
}
}

