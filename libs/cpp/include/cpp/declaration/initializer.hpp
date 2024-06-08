#ifndef CPP_DECLARATION_INITIALIZER_H
#define CPP_DECLARATION_INITIALIZER_H

#include <string>
#include <optional>
#include <variant>
#include <vector>

#include "cpp/symbols.hpp"


namespace cpp {
    class expression_t {
    public:
        expression_t(const std::string& expr) : expression_(expr) {}
        const std::string& expression() const { return expression_; }
    private:
        std::string expression_;
    };

    class expression_list_t {
    public:
        expression_list_t() {}
        static constexpr auto separator = cpp::symbols::comma;
        const std::vector<expression_t> expressions() const { return list; }
    private:
        std::vector<expression_t> list;
    };

    class copy_initialization_t {
    public:
        copy_initialization_t(expression_t expression) : expr_(expression) {}
        static constexpr auto assignment = cpp::symbols::assignment;
        expression_t expression() const { return expr_; }
    private:
        expression_t expr_;
    };

    class initializer_list_t;

    using initializer_clause_t = std::variant<expression_t, initializer_list_t>;

    class initializer_list_t {
    public:
        initializer_list_t(initializer_clause_t init_clause);
        static constexpr auto separator = cpp::symbols::comma;
        const auto& list() const { return l_; }
    private:
        std::vector<initializer_clause_t> l_;
    };

    class braced_enclosed_initializer_list_t {
    public:
        static constexpr auto open  = cpp::symbols::open_curly_brace;
        static constexpr auto close = cpp::symbols::close_curly_brace;
        const auto& init_list() const { return initializer_list; }
    private:
        std::optional<initializer_list_t> initializer_list;
    };

    class list_initialization_t {
    public:
        // Here this is optional field, that's why I am going to not to use assignment.
        bool assign = false;
        static constexpr auto assignment = cpp::symbols::assignment;
        const auto& init_list() const { return initializer_list; }
    private:
        braced_enclosed_initializer_list_t initializer_list;
    };

    class direct_initialization_t {
    public:
        static constexpr auto open  = cpp::symbols::open_brace;
        static constexpr auto close = cpp::symbols::close_brace;
        const auto& list() const { return l_; }
    private:
        std::variant<expression_list_t, initializer_list_t> l_;
    };

    using initializer_t = std::variant<copy_initialization_t, list_initialization_t, direct_initialization_t>;
}

#endif /* CPP_DECLARATION_INITIALIZER_H */
