#pragma once

#include <string>
#include <variant>

namespace language {

    class space_t {
    public:
        static constexpr std::string symbol  = " ";
    };

    class collon_t {
    public:
        static constexpr std::string symbol = ":";
    };

    class semi_collon_t {
    public:
        static constexpr std::string symbol = ";";
    };

    class ampersand_t {
    public:
        static constexpr std::string symbol = "&";
    };

    class open_curly_brace_t {
    public:
        static constexpr std::string symbol = "{";
    };

    class close_curly_brace_t {
    public:
        static constexpr std::string symbol = "}";
    };

    class open_brace_t {
    public:
        static constexpr std::string symbol = "(";
    };

    class close_brace_t {
    public:
        static constexpr std::string symbol = ")";
    };

    class assignment_t {
    public:
        static constexpr std::string symbol = "=";
    };

    class comma_t {
    public:
        static constexpr std::string symbol = ",";
    };

    class newline_t {
    public:
        static constexpr std::string symbol = "\n";
    };

    using symbol_t = std::variant<space_t, collon_t, semi_collon_t,
                                  ampersand_t,
                                  open_curly_brace_t, close_curly_brace_t,
                                  open_brace_t, close_brace_t,
                                  assignment_t, comma_t, newline_t>;
}
