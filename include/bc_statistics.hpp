#ifndef BC_STATISTICS_H
#define BC_STATISTICS_H
#include <chrono>
#include <list>
#include <string>

namespace bicycle_engine::misc::statistics {
    using tp = std::chrono::time_point<std::chrono::system_clock>;
    
    class Statistics {
    public:
        Statistics() = default;
        void add_frame();
        std::string get_statistics();
    private:
        tp start_block = std::chrono::system_clock::now();
        std::list<tp> bucket;
    };
}

#endif /* BC_STATISTICS_H */
