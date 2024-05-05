#include "slow_device.hpp"

int main() {
    std::atomic_bool run(false);
    SlowDevice sd;

    output_current_time("Start time:");
    sd.get_hello([&run](auto msg) {
            output_current_time("End time:");
            std::cout << msg << "\n";
            run.store(true);
            run.notify_one();
        });

    run.wait(false);

    return 0;
}
