#ifndef BICYCLE_ENGINE_WAYLAND_INTERFACE_H
#define BICYCLE_ENGINE_WAYLAND_INTERFACE_H
#include <cstdint>
#include <ctime>
#include <string>

namespace bicycle_engine::wayland::interface {

    // TODO: Make WLInterface template class which is going to specify that derived classes are non-copyable and movable with requirements
    class WLInterface {
    public:
        std::string print() const;
        const std::string& interface() const { return interface_; }
        uint32_t name() const { return name_; }
        uint32_t version() const { return version_; }
        std::time_t update_time() const { return update_time_; }
    protected:
        WLInterface();
        WLInterface(uint32_t name, const char *interface, uint32_t version);
        // Not copyable
        WLInterface(const WLInterface&) = delete;
        WLInterface& operator=(const WLInterface&) = delete;
        // Movable
        WLInterface(WLInterface&&);
        WLInterface& operator=(WLInterface&&);

        std::string interface_ = "";
        uint32_t name_;
        uint32_t version_;
        std::time_t update_time_ = std::time(nullptr);
    private:
        void swap(WLInterface&, WLInterface&);
    };
}

#endif /* BICYCLE_ENGINE_WAYLAND_INTERFACE_H */
