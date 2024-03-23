#ifndef BC_LOGGER_LOGGER_H
#define BC_LOGGER_LOGGER_H
#include <string>
#include <format>
#include <source_location>
#include <iostream>

namespace bicycle_engine::logger {
    template<typename... Ts>
    class Logger {
    public:
        Logger(std::string_view format_str, Ts&&... ts, const std::source_location& loc = std::source_location::current()) {
            auto prefix = std::format("{}:{}[{}] INFO:", loc.file_name(), loc.line(), loc.function_name());
            auto message = std::vformat(format_str, std::make_format_args(std::forward<Ts>(ts)...)); 
            std::cout << prefix << message << "\n";
        }

        Logger(const std::source_location& loc = std::source_location::current()) {
            auto prefix = std::format("{}:{}[{}] >> ", loc.file_name(), loc.line(), loc.function_name());
            std::cout << prefix << "\n";
        }
    };

    // NOTE: It is nice hack but if I use object for logging it is going to waste memory on each logging since it is going to occupy 1byte for each logging call.
    // For now I am going to leave it as is, but try to think of other ways to make it work
    // So it is maybe better to think of other ways to use it.
    template<typename... Ts>
    Logger(std::string_view, Ts&&...) -> Logger<Ts...>;
}

#endif
