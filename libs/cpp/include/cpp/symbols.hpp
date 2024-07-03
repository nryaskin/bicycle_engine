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

    class double_quote_t {
    public:
        constexpr double_quote_t() {}
        static constexpr std::string symbol = "\"";
    };

    class open_angle_t {
    public:
        constexpr open_angle_t() {}
        static constexpr std::string symbol = "<";
    };

    class close_angle_t {
    public:
        constexpr close_angle_t() {}
        static constexpr std::string symbol = ">";
    };

    class solidus_t {
    public:
        constexpr solidus_t() {}
        static constexpr std::string symbol = "/";
    };

    class reverse_solidus_t {
    public:
        constexpr reverse_solidus_t() {}
        static constexpr std::string symbol = "\\";
    };

    class asterisk_t {
    public:
        constexpr asterisk_t() {}
        static constexpr std::string symbol = "*";
    };

    using symbol_t = std::variant<space_t, collon_t, semi_collon_t,
                                  ampersand_t,
                                  open_curly_brace_t, close_curly_brace_t,
                                  open_brace_t, close_brace_t,
                                  assignment_t, comma_t, newline_t,
                                  double_quote_t,
                                  open_angle_t, close_angle_t,
                                  solidus_t, reverse_solidus_t, asterisk_t>;
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
    static constexpr symbol_t double_quote = double_quote_t {};
    static constexpr symbol_t open_angle = open_angle_t {};
    static constexpr symbol_t close_angle = close_angle_t {};
    static constexpr symbol_t solidus = solidus_t {};
    static constexpr symbol_t reverse_solidus = reverse_solidus_t {};
    static constexpr symbol_t asterisk = asterisk_t {};
}
