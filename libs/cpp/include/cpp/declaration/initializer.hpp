#ifndef CPP_DECLARATION_INITIALIZER_H
#define CPP_DECLARATION_INITIALIZER_H

#include <string>
#include <optional>
#include <variant>
#include <vector>

#include "cpp/symbols.hpp"
#include "cpp/specification.hpp"
#include "cpp/format.hpp"


namespace cpp {
    class expression_t {
    public:
        expression_t() = default;
        expression_t(const std::string& expr) : expression_(expr) {}
        const std::string& expression() const { return expression_; }
    private:
        std::string expression_;
    };

    class expression_list_t : public std::vector<expression_t> {
    public:
        using format = language::format_t<expression_t, language::zero_or_more<language::comma_t, expression_t>>;
        expression_list_t() {}
        void sequential_all(auto&& action) {
            auto it = begin();
            action(*it);
            while(++it != end()) {
                action(language::comma);
                action(*it);
            }
        }
        static constexpr auto separator = language::comma_t {};
    };

    class copy_initialization_t {
    public:
        copy_initialization_t(expression_t expression) : expr_(expression) {}

        void sequential_all(auto&& action) const {
            action(language::assignment);
            action(language::space);
            action(expr_);
        }

    private:
        expression_t expr_;
    };

    class initializer_list_t;

    using initializer_clause_t = std::variant<expression_t, initializer_list_t>;

    class initializer_list_t {
    public:
        initializer_list_t(initializer_clause_t init_clause);
        static constexpr auto separator = language::comma_t {};
        const auto& list() const { return l_; }
    private:
        std::vector<initializer_clause_t> l_;
    };

    class braced_enclosed_initializer_list_t {
    public:
        static constexpr auto open  = language::open_curly_brace_t {};
        static constexpr auto close = language::close_curly_brace_t {};
        const auto& init_list() const { return initializer_list; }
    private:
        std::optional<initializer_list_t> initializer_list;
    };

    class list_initialization_t {
    public:
        // Here this is optional field, that's why I am going to not to use assignment.
        bool assign = false;
        static constexpr auto assignment = language::assignment_t {};
        const auto& init_list() const { return initializer_list; }
        void sequential_all(auto&& action) const {

        }
    private:
        braced_enclosed_initializer_list_t initializer_list;
    };

    class direct_initialization_t {
    public:
        static constexpr auto open  = language::open_brace_t {};
        static constexpr auto close = language::close_brace_t {};
        const auto& list() const { return l_; }
        void sequential_all(auto&& action) const {}
    private:
        std::variant<expression_list_t, initializer_list_t> l_;
    };

    using initializer_t = std::variant<copy_initialization_t, list_initialization_t, direct_initialization_t>;
}

#endif /* CPP_DECLARATION_INITIALIZER_H */
