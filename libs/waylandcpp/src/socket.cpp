#include "waylandcpp/wire/socket.hpp"

#include <array>
#include <cassert>
#include <cstring>
#include <iostream>
#include <unistd.h>
#include <cerrno>
#include <format>


namespace waylandcpp::wire {
    inline void assert_err(int ret) {
        std::cout << "ret: " << ret << "; errno: " << errno << std::endl;
        assert(ret != -1);
    }

    std::string WAYLAND_DISPLAY("/run/user/1000/wayland-0");

    WLSocket::WLSocket() {
        int ret;
        fd = socket(AF_UNIX, SOCK_STREAM, 0);        
        if (fd < 0) {
            throw std::runtime_error("Cannot open socket");
        }
        addr.sun_family = AF_UNIX;
        std::strncpy(addr.sun_path, WAYLAND_DISPLAY.data(), WAYLAND_DISPLAY.size() + 1);
        ret = connect(fd, reinterpret_cast<struct sockaddr*>(&addr), sizeof(decltype(addr))); 
        assert_err(ret);
    }

    void WLSocket::write(const uint8_t* data, uint32_t size) {
        int ret = ::write(fd, data, size);
        if (ret < 0) {
            throw std::runtime_error(std::format("Error while write: {:x}, {}", errno, std::strerror(errno)));
        }
    }

    std::vector<uint8_t> WLSocket::read() {
        std::vector<uint8_t> buffer;
        std::array<uint8_t, 1024> buf;
        int ret = buf.size();

        // Q: Can it go on forever if there is too much data in socket?
        while(ret == buf.size()) {
            ret = ::read(fd, buf.data(), buf.size());
            assert_err(ret);
            buffer.insert(buffer.end(), buf.begin(), buf.begin() + ret);
        }
        assert(buffer.size() % 4 == 0);
        return std::move(buffer);
    }

    WLSocket::~WLSocket() {
        close(fd);
    }
}
