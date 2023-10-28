#ifndef BC_WAYLAND_SHM_H
#define BC_WAYLAND_SHM_H
#include <stdexcept>
#include <string>
#include <stdint.h>
#include <sys/mman.h>


namespace bicycle_engine::wayland::memory {
    class FailedToOpenShmException : public std::runtime_error {
    public:
        FailedToOpenShmException() : std::runtime_error::runtime_error("Failed to open shared memory object") {}
    };

    class SharedMemory {
    public:
        SharedMemory(size_t size);
        SharedMemory(const SharedMemory&) = delete;
        SharedMemory(SharedMemory&&);

        SharedMemory& operator=(const SharedMemory&) = delete;
        SharedMemory& operator=(SharedMemory&&);
        uint32_t& operator[](size_t index);

        int get_fd() const { return fd; }
        ~SharedMemory();
    private:
        // @brief Generates shared memory file name of form /BC-Wayland-xxxxxx
        static std::string generate_shm_file_name(uint32_t idx);
        // @brief Create shared file.
        //
        // @param size - size of created file
        static int create_shm_file(int size);
        // Shared memory file idx
        // I don't need it right now but if I am going to have
        // more than one instantiation then it is better to synchronize it.
        static uint32_t shared_file_idx;
        int fd;
        size_t size;
        uint32_t* data;
        std::string name;
    };
}

#endif /* BC_WAYLAND_SHM_H */
