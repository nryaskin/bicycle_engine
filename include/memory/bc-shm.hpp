#ifndef BICYCLE_ENGINE_MEMORY_BC_SHM_H
#define BICYCLE_ENGINE_MEMORY_BC_SHM_H
#include <stdexcept>
#include <string>
#include <stdint.h>
#include <format>
#include <sys/mman.h>
#include "memory/bc-shm-file.hpp"

namespace bicycle_engine::memory {
    class SharedMemory {
    public:
        SharedMemory(size_t size = 0);

        SharedMemory(const SharedMemory&) = delete;
        SharedMemory& operator=(const SharedMemory&) = delete;

        SharedMemory(SharedMemory&&);
        SharedMemory& operator=(SharedMemory&&);

        // resize shared memory region
        void resize(size_t size_);

        // return memory size_
        inline size_t get_size() const {
            return size;
        }

        // return underlying file descriptor
        inline SharedFile::file_descriptor_t get_fd() const {
            return shared_file.get_fd();
        }

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

           return static_cast<MemoryUnit*>(data)[index];
        }

        ~SharedMemory();
    private:
        // @brief Generates shared memory file name of form /BC-Wayland-xxxxxx
        static const std::string generate_shm_file_name();
        SharedFile shared_file;

        // TODO: Replace this with using random generated name later;
        static uint32_t shared_file_idx;
        size_t size;
        void* data;
    };
}

#endif /* BICYCLE_ENGINE_MEMORY_BC_SHM_H */
