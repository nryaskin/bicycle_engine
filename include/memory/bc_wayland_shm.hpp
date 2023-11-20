#ifndef BC_MEMORY_SHM_H
#define BC_MEMORY_SHM_H
#include <stdexcept>
#include <string>
#include <stdint.h>
#include <format>
#include <sys/mman.h>
#include <wayland-client-protocol.h>

namespace bicycle_engine::wayland::memory {
    namespace wrapper {
        // @brief map shared fd to memory region
        //
        // @param fd - open file descriptor
        // @param size - size of required memory region
        // @throws bicycle_engine::wayland::memory::error::ShmMmapException Thrown if underlying mmap failed due to any reason.
        static void* map_data(int fd, size_t size);

        // @brief unmap shared fd to memory region
        //
        // @param fd - open file descriptor
        // @param size - size of required memory region
        // @throws bicycle_engine::wayland::memory::error::ShmMunmapException Thrown if underlying mmap failed due to any reason.
        static void munmap_data(int fd, size_t size);

        // @brief truncates file descriptor of mapped memory region 
        //
        // @param fd - open file descriptor
        // @param size - size of required memory region
        // @throws bicycle_engine::wayland::memory::error::ShmTruncException Thrown if underlying ftrunc failed due to any reason.
        static void ftruncate_data(int fd, size_t size);
    };

    class SharedMemory {
    public:
        SharedMemory(size_t size = 0);

        SharedMemory(const SharedMemory&) = delete;
        SharedMemory& operator=(const SharedMemory&) = delete;

        SharedMemory(SharedMemory&&);
        SharedMemory& operator=(SharedMemory&&);

        // resize shared memory region
        void resize(size_t size_);

        template<std::integral MemoryUnit>
        MemoryUnit& operator[](size_t index) {
           if (index >= size) {
               throw std::out_of_range(std::format("Index: {} out of range: {}!", index, size));
           }

           return static_cast<MemoryUnit>(data)[index];
        }

        template<std::integral MemoryUnit>
        const MemoryUnit& operator[](size_t index) const {
           if (index >= size) {
                //throw out of bound exception
           }

           return static_cast<MemoryUnit>(data)[index];
        }

        int get_fd() const { return fd; }
        ~SharedMemory();
    private:
        // @brief Generates shared memory file name of form /BC-Wayland-xxxxxx
        static const std::string generate_shm_file_name();

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
        void* data;
    };
}

#endif /* BC_MEMORY_SHM_H */
