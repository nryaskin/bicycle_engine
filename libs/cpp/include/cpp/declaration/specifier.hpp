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
    class specifier_t : public language::extensible_t {};

    template<typename T>
    class keyword_specifier_t : public specifier_t {
    public:
        static constexpr auto keyword = T {};
    };

    class static_specifier_t : public keyword_specifier_t<language::static_keyword_t> {};

    class extern_specifier_t : public keyword_specifier_t<language::extern_keyword_t> {};

    class constexpr_specifier_t : public keyword_specifier_t<language::constexpr_keyword_t> {};

    class volatile_qualifier_t : public keyword_specifier_t<language::volatile_keyword_t> {};

    class const_qualifier_t : public keyword_specifier_t<language::const_keyword_t> {};

    class cv_qualifier_t {
        std::optional<const_qualifier_t> const_qualifier;
        std::optional<volatile_qualifier_t> volatile_qualifier;
    };

    // types such as char, bool, short, int, long, double, etc., previously declared enum, class name. And even more but I don't care.
    class simple_type_specifier_t : public specifier_t {
    public:
        simple_type_specifier_t(const std::string& id) : id_(id) {}
        const std::string& id() const { return id_; }
    private:
        std::string id_;
    };

    // Sequence of white space separated specifiers
    // vector
    // (<specifier>, <symbol>,...)
    class decl_specifier_seq_t : public language::extensible_list {
    public:
        using format = language::format_t<specifier_t, language::zero_or_more<language::space_t, specifier_t>>;

        explicit decl_specifier_seq_t(auto ...specifiers) {
            (push_back(specifiers),...);
        }

        void remove(int i) {
            erase(begin() + i);
        }

    public:
        static constexpr auto separator = language::space_t {};
    };
}

#endif /* CPP_DECLARATION_SPECIFIER_H */
