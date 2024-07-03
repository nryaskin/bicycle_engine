#ifndef BE_EXPERIMENTS_ASIO_SLOW_DEVICE_H
#define BE_EXPERIMENTS_ASIO_SLOW_DEVICE_H

#include <thread>
#include <functional>
#include <chrono>
#include <iostream>
#include <ctime>

#include <boost/asio.hpp>

class SlowDevice {
public:
    using callback_t = std::function<void(std::string)>;
    SlowDevice() {}

    void call(callback_t&& cb) {
        using namespace std::chrono_literals;
        std::this_thread::sleep_for(5s);
        std::string msg("Hello, World!");
        cb(msg);
    }

    void get_hello(callback_t&& cb) {
        t = std::thread(&SlowDevice::call, this, cb);
    }

    ~SlowDevice() {
        t.join();
    }
    std::thread t;
};

inline void output_current_time(std::string&& message) {
    auto current = std::chrono::system_clock::now();
    std::time_t current_time = std::chrono::system_clock::to_time_t(current);
    std::cout << message << std::ctime(&current_time) << "\n";
}

#endif /* BE_EXPERIMENTS_ASIO_SLOW_DEVICE_H */
