#include "memory/bc_wayland_shm_file.hpp"
#include "memory/bc_error.hpp"
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <filesystem>

namespace bicycle_engine::wayland::memory {
    SharedFile::SharedFile(const std::string& name_) : fd {-1} , name {name_} {
        fd = shm_open(name.c_str(), O_RDWR | O_CREAT | O_EXCL | O_TRUNC, 0600);

        if (fd < 0) {
            std::filesystem::path p(name);
            throw error::ShmOpenException(p, errno);
        }
    }

    // TODO: Make move constructor thread safe to guarantee const correctness.
    SharedFile::SharedFile(SharedFile&& other) : fd { -1 } {
        std::swap(fd, other.fd);
        std::swap(name, other.name);
    }

    // TODO: Make move assignment operator thread safe to guarantee const correctness.
    SharedFile& SharedFile::operator=(SharedFile&& other) {
        std::swap(fd, other.fd);
        std::swap(name, other.name);
        return *this;
    }

    void SharedFile::truncate(size_t size) {
        if (ftruncate(fd, size) == -1) {
            throw error::ShmTruncException(fd, size, errno);
        }
    }

    SharedFile::file_descriptor_t SharedFile::get_fd() const {
        return fd;
    }

    SharedFile::~SharedFile() {
        if (fd >= 0) {
            close(fd);
            fd = -1;
        }

        if (name.size()) {
            shm_unlink(name.c_str());
        }
    }
}


