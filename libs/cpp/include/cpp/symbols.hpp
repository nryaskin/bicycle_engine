#pragma once

#include <string>
#include <variant>

namespace language {

    class space_t {
    public:
        constexpr space_t() {}
        static constexpr std::string symbol  = " ";
    };

    class collon_t {
    public:
        constexpr collon_t() {}
        static constexpr std::string symbol = ":";
    };

    class semi_collon_t {
    public:
        constexpr semi_collon_t() {}
        static constexpr std::string symbol = ";";
    };

    class ampersand_t {
    public:
        constexpr ampersand_t() {}
        static constexpr std::string symbol = "&";
    };

    class open_curly_brace_t {
    public:
        constexpr open_curly_brace_t() {}
        static constexpr std::string symbol = "{";
    };

    class close_curly_brace_t {
    public:
        constexpr close_curly_brace_t() {}
        static constexpr std::string symbol = "}";
    };

    class open_brace_t {
    public:
        constexpr open_brace_t() {}
        static constexpr std::string symbol = "(";
    };

    class close_brace_t {
    public:
        constexpr close_brace_t() {}
        static constexpr std::string symbol = ")";
    };

    class assignment_t {
    public:
        constexpr assignment_t() {}
        static constexpr std::string symbol = "=";
    };

    class comma_t {
    public:
        constexpr comma_t() {}
        static constexpr std::string symbol = ",";
    };

    class newline_t {
    public:
        constexpr newline_t() {}
        static constexpr std::string symbol = "\n";
    };

    using symbol_t = std::variant<space_t, collon_t, semi_collon_t,
                                  ampersand_t,
                                  open_curly_brace_t, close_curly_brace_t,
                                  open_brace_t, close_brace_t,
                                  assignment_t, comma_t, newline_t>;
    static constexpr symbol_t space = space_t {};
    static constexpr symbol_t collon = collon_t {};
    static constexpr symbol_t semi_collon = semi_collon_t {};
    static constexpr symbol_t ampersand = ampersand_t {};
    static constexpr symbol_t open_curly_brace = open_curly_brace_t {};
    static constexpr symbol_t close_curly_brace = close_curly_brace_t {};
    static constexpr symbol_t open_brace = open_brace_t {};
    static constexpr symbol_t close_brace = close_brace_t {};
    static constexpr symbol_t assignment = assignment_t {};
    static constexpr symbol_t comma = comma_t {};
    static constexpr symbol_t newline = newline_t {};
}
