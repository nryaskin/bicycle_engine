#include "bc_statistics.hpp"
#include <format>

using namespace std::chrono_literals;

namespace bicycle_engine::misc::statistics {
void Statistics::add_frame() {
    auto current_time = std::chrono::system_clock::now();
    auto start_block = bucket.begin();

    while (start_block != bucket.end()
           && current_time - (*start_block) > 1s) {
        bucket.pop_front();
        start_block = bucket.begin();
    }
    bucket.emplace_back(current_time);
}

std::string Statistics::get_statistics() {
    return std::format("Frames Per Second: {}", bucket.size()); 
}
}
