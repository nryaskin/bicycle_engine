#ifndef BICYCLE_ENGINE_SHM_FILE_H
#define BICYCLE_ENGINE_SHM_FILE_H

#include <string>

namespace bicycle_engine::memory {
    // Wrapper for managing shared files.
    class SharedFile {
    public:
        using file_descriptor_t = int;
        SharedFile() = default;
        SharedFile(const std::string& name);
        SharedFile(const SharedFile&) = delete;
        SharedFile(SharedFile&&);
        ~SharedFile();

        void truncate(size_t size);
        file_descriptor_t get_fd() const;

        SharedFile& operator=(const SharedFile&) = delete;
        SharedFile& operator=(SharedFile&&);
    private:
        file_descriptor_t fd = -1; 
        std::string name;
        size_t size = 0;
    };
}

#endif /* BICYCLE_ENGINE_SHM_FILE_H */
