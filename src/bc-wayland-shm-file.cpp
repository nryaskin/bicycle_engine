#include "memory/bc_wayland_shm_file.hpp"
#include "memory/bc_error.hpp"
#include "logger/logger.hpp"
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <filesystem>

namespace bicycle_engine::wayland::memory {
    SharedFile::SharedFile(const std::string& name_) : fd {-1} , name {name_} {
        bicycle_engine::logger::Logger("shared file name: {}", name);

        fd = shm_open(name.c_str(), O_RDWR | O_CREAT | O_EXCL | O_TRUNC, 0600);

        if (fd < 0) {
            std::filesystem::path p(name);
            throw error::ShmOpenException(p, errno);
        }

        shm_unlink(name.c_str());
    }

    // TODO: Make move constructor thread safe to guarantee const correctness.
    SharedFile::SharedFile(SharedFile&& other) : fd { -1 } {
        bicycle_engine::logger::Logger();
        std::swap(fd, other.fd);
        std::swap(name, other.name);
    }

    // TODO: Make move assignment operator thread safe to guarantee const correctness.
    SharedFile& SharedFile::operator=(SharedFile&& other) {
        bicycle_engine::logger::Logger("old fd: {}, old name: {}", fd, name);
        std::swap(fd, other.fd);
        std::swap(name, other.name);
        bicycle_engine::logger::Logger("new fd: {}, new name: {}", fd, name);
        return *this;
    }

    void SharedFile::truncate(size_t size) {
        bicycle_engine::logger::Logger("size: {}", size);
        if (ftruncate(fd, size) == -1) {
            throw error::ShmTruncException(fd, size, errno);
        }
    }

    SharedFile::file_descriptor_t SharedFile::get_fd() const {
        return fd;
    }

    SharedFile::~SharedFile() {
        bicycle_engine::logger::Logger("fd: {}, name: {}", fd, name);
        if (fd >= 0) {
            close(fd);
            fd = -1;
        }
    }
}


