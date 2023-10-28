#include "bc_wayland_shm.hpp"
#include <format>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string>
#include <iostream>

namespace {
    constexpr std::string WAYLAND_SHM_PREFIX = "/BC-Wayland";
}

namespace bicycle_engine::wayland::memory {
// Initialize it here, later think if it is good decision.
uint32_t SharedMemory::shared_file_idx = 0;

SharedMemory::SharedMemory(size_t size): fd(-1), size(size), data(nullptr) {
    name = SharedMemory::generate_shm_file_name(SharedMemory::shared_file_idx++);
    fd =  shm_open(name.c_str(), O_RDWR | O_CREAT | O_EXCL, 0600);

    if (fd < 0) {
        // Pass name to this exception
        throw FailedToOpenShmException();
    }

    ftruncate(fd, size); 
    data = static_cast<uint32_t*>(mmap(nullptr, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0));
}

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

uint32_t& SharedMemory::operator[](size_t index) {
   if (index >= size) {
        //thorw out of bound exception
   }

   return data[index];
}

std::string SharedMemory::generate_shm_file_name(uint32_t idx) {
    return std::move(std::format("{}-{}", ::WAYLAND_SHM_PREFIX, idx));
}

SharedMemory::~SharedMemory() {
    munmap(data, size);
    data = nullptr;
    size = 0;
    close(fd);
    fd = -1;
    shm_unlink(name.c_str());
    name =  "";
}
}

