#ifndef CPP_DECLARATION_SPECIFIER_H
#define CPP_DECLARATION_SPECIFIER_H

#include <variant>
#include <string>
#include <format>
#include <vector>

#include "cpp/symbols.hpp"
#include "cpp/keywords.hpp"
#include "cpp/specification.hpp"
#include "cpp/format.hpp"

// constexpr static char  | * const a | = nullptr;
// decl-specifier-seq     |   init-declarator-list
namespace cpp {
    class keyword_specifier_t {
    public:
        language::keyword_t keyword;
        constexpr void sequential_all(auto&& action) const {
            action(keyword);
        }
    };

    class static_specifier_t : public keyword_specifier_t {
    public:
        constexpr static_specifier_t() {
            keyword = language::static_keyword;
        }
    };

    class extern_specifier_t : public keyword_specifier_t {
    public:
        constexpr extern_specifier_t() {
            keyword = language::extern_keyword;
        }
    };

    class constexpr_specifier_t : public keyword_specifier_t {
    public:
        constexpr constexpr_specifier_t() {
            keyword = language::constexpr_keyword;
        }
    };

    class volatile_qualifier_t : public keyword_specifier_t {
    public:
        constexpr volatile_qualifier_t() {
            keyword = language::volatile_keyword;
        }
    };

    class const_qualifier_t : public keyword_specifier_t {
    public:
        constexpr const_qualifier_t() {
            keyword = language::const_keyword;
        }
    };

    class cv_qualifier_t {
    public:
        cv_qualifier_t() {}
        void sequential_all(auto&& action) const {
            if (const_qualifier) {
                action(const_qualifier.value());
            }
            if (volatile_qualifier) {
                action(volatile_qualifier.value());
            }
        }
    private:
        std::optional<const_qualifier_t> const_qualifier;
        std::optional<volatile_qualifier_t> volatile_qualifier;
    };

    // types such as char, bool, short, int, long, double, etc., previously declared enum, class name. And even more but I don't care.
    class simple_type_specifier_t {
    public:
        simple_type_specifier_t(const std::string& id) : id_(id) {}
        const std::string& id() const { return id_; }
    private:
        std::string id_;
    };

    static constexpr auto static_specifier = static_specifier_t {};
    static constexpr auto extern_specifier = extern_specifier_t {};
    static constexpr auto constexpr_specifier = constexpr_specifier_t {};
    static constexpr auto volatile_qualifier = volatile_qualifier_t {};
    static constexpr auto const_qualifier = const_qualifier_t {};

    using specifier_t = std::variant<static_specifier_t, 
                                     extern_specifier_t, 
                                     constexpr_specifier_t,
                                     volatile_qualifier_t,
                                     const_qualifier_t,
                                     simple_type_specifier_t>;

    // Sequence of white space separated specifiers
    // vector
    // (<specifier>, <symbol>,...)
    class decl_specifier_seq_t : public std::vector<specifier_t> {
    public:
        decl_specifier_seq_t(auto ...specifiers) {
            (push_back(specifiers),...);
        }

        void sequential_all(auto&& action) const {
            auto it = begin();
            action(*it);
            while(++it != end()) {
                action(separator);
                action(*it);
            }
        }

    public:
        static constexpr auto separator = language::space;
    };
}

#endif /* CPP_DECLARATION_SPECIFIER_H */
