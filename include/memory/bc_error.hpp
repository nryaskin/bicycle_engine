#ifndef BC_MEMORY_ERROR_H
#define BC_MEMORY_ERROR_H
#include <stdexcept>
#include <filesystem>
#include <format>
#include <string>


namespace bicycle_engine::wayland::memory::error {
    class ShmException : public std::exception {
    public:
        ShmException(int error_code_) : error_code(error_code_) {

        }

        virtual const char* what() const noexcept override {
            return message.c_str();
        };

        // Operation error code(errno)
        int error_code;
        std::string message;
    };

    class ShmOpenException : public ShmException {
    public:
        ShmOpenException(std::filesystem::path& path_, int error_code_) : ShmException(error_code_), path(path_) {
            message = std::format("Shared Memory Open exception! File name: {}, error_code = {}", path.c_str(), error_code); 
        }

        // Path to file that was used in shm_open
        std::filesystem::path path;
    };

    class ShmMmapException : public ShmException {
    public:
        ShmMmapException(int fd_, int size_, int prot_, int flags_, int error_code_) : ShmException(error_code_), fd(fd_), size(size_), prot(prot_), flags(flags_) {
            message = std::format("Shared Memory MMAP exception! fd: {}, size = {}, prot = {:#X}, flags = {:#X}, error_code = {}", fd, size, prot, flags, error_code); 
        }
    protected:
        int fd;
        int size;
        int prot;
        int flags;
    };

    class ShmMunmapException : public ShmException {
    public:
        ShmMunmapException(int size_, int error_code_) : ShmException(error_code_), size(size_) {
            message = std::format("Shared Memory munmap exception! size = {}, error_code = {}", size, error_code); 
        }
    protected:
        int size;
    };

    class ShmTruncException : public ShmException {
    public:
        ShmTruncException(int fd_, int size_, int error_code_) : ShmException(error_code_), fd(fd_), size(size_) {
            message = std::format("Shared Memory ftruncate exception! fd: {}, size = {}, error_code = {}", fd, size, error_code); 
        }
    protected:
        int fd;
        int size;
    };
}


#endif /* BC_MEMORY_ERROR_H */
