#pragma once

#include <string>
#include <variant>

namespace language {
    class static_keyword_t {
    public:
        static constexpr std::string keyword = "static";
    };

    class extern_keyword_t {
    public:
        static constexpr std::string keyword = "extern";
    };

    class constexpr_keyword_t {
    public:
        static constexpr std::string keyword = "constexpr";
    };

    class volatile_keyword_t {
    public:
        static constexpr std::string keyword = "volatile";
    };

    class const_keyword_t {
    public:
        static constexpr std::string keyword = "const";
    };

    class if_keyword_t {
    public:
        static constexpr std::string keyword = "if";
    };

    class switch_keyword_t {
    public:
        static constexpr std::string keyword = "switch";
    };

    class case_keyword_t {
    public:
        static constexpr std::string keyword = "case";
    };

    class for_keyword_t {
    public:
        static constexpr std::string keyword = "for";
    };

    class while_keyword_t {
    public:
        static constexpr std::string keyword = "while";
    };

    using keyword_t = std::variant<static_keyword_t, extern_keyword_t,
                                   constexpr_keyword_t, volatile_keyword_t,
                                   const_keyword_t,
                                   if_keyword_t, switch_keyword_t,
                                   case_keyword_t,
                                   for_keyword_t, while_keyword_t>;
};
