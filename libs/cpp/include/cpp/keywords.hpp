#pragma once

#include <string>
#include <variant>

namespace language {
    class static_keyword_t {
    public:
        constexpr static_keyword_t() {}
        static constexpr std::string keyword = "static";
    };

    class extern_keyword_t {
    public:
        constexpr extern_keyword_t() {}
        static constexpr std::string keyword = "extern";
    };

    class constexpr_keyword_t {
    public:
        constexpr constexpr_keyword_t() {}
        static constexpr std::string keyword = "constexpr";
    };

    class volatile_keyword_t {
    public:
        constexpr volatile_keyword_t() {}
        static constexpr std::string keyword = "volatile";
    };

    class const_keyword_t {
    public:
        constexpr const_keyword_t() {}
        static constexpr std::string keyword = "const";
    };

    class if_keyword_t {
    public:
        constexpr if_keyword_t() {}
        static constexpr std::string keyword = "if";
    };

    class switch_keyword_t {
    public:
        constexpr switch_keyword_t() {}
        static constexpr std::string keyword = "switch";
    };

    class case_keyword_t {
    public:
        constexpr case_keyword_t() {}
        static constexpr std::string keyword = "case";
    };

    class for_keyword_t {
    public:
        constexpr for_keyword_t() {}
        static constexpr std::string keyword = "for";
    };

    class while_keyword_t {
    public:
        constexpr while_keyword_t() {}
        static constexpr std::string keyword = "while";
    };

    class default_keyword_t {
    public:
        constexpr default_keyword_t() {}
        static constexpr std::string keyword = "default";
    };

    class goto_keyword_t {
    public:
        constexpr goto_keyword_t() {}
        static constexpr std::string keyword = "goto";
    };
    
    class break_keyword_t {
    public:
        constexpr break_keyword_t() {}
        static constexpr std::string keyword = "break";
    };

    class continue_keyword_t {
    public:
        constexpr continue_keyword_t() {}
        static constexpr std::string keyword = "continue";
    };

    class return_keyword_t {
    public:
        constexpr return_keyword_t() {}
        static constexpr std::string keyword = "return";
    };

    using keyword_t = std::variant<static_keyword_t, extern_keyword_t,
                                   constexpr_keyword_t, volatile_keyword_t,
                                   const_keyword_t,
                                   if_keyword_t, switch_keyword_t,
                                   case_keyword_t,
                                   for_keyword_t, while_keyword_t,
                                   default_keyword_t, goto_keyword_t,
                                   break_keyword_t, continue_keyword_t, return_keyword_t>;

    static constexpr keyword_t static_keyword = static_keyword_t {};
    static constexpr keyword_t extern_keyword = extern_keyword_t {};
    static constexpr keyword_t constexpr_keyword = constexpr_keyword_t {};
    static constexpr keyword_t volatile_keyword = volatile_keyword_t {};
    static constexpr keyword_t const_keyword = const_keyword_t {};
    static constexpr keyword_t if_keyword = if_keyword_t {};
    static constexpr keyword_t switch_keyword = switch_keyword_t {};
    static constexpr keyword_t case_keyword = case_keyword_t {};
    static constexpr keyword_t for_keyword = for_keyword_t {};
    static constexpr keyword_t while_keyword = while_keyword_t {};
    static constexpr keyword_t default_keyword = default_keyword_t {};
    static constexpr keyword_t goto_keyword = goto_keyword_t {};
    static constexpr keyword_t break_keyword = break_keyword_t {};
    static constexpr keyword_t continue_keyword = continue_keyword_t {};
    static constexpr keyword_t return_keyword = return_keyword_t {};
};
