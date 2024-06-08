#ifndef CPP_DECLARATION_SPECIFIER_H
#define CPP_DECLARATION_SPECIFIER_H

#include <variant>
#include <string>
#include <format>
#include <vector>

#include "cpp/symbols.hpp"

// constexpr static char  | * const a | = nullptr;
// decl-specifier-seq     |   init-declarator-list
namespace cpp {
    class storage_specifier_t {};
    class static_specifier_t : storage_specifier_t {
    public:
        static constexpr std::string keyword = "static";
    };
    static_specifier_t static_specifier;

    class extern_specifier_t : storage_specifier_t {
    public:
        static constexpr std::string keyword = "extern";
    };
    extern_specifier_t extern_specifier;

    class constexpr_specifier_t { 
    public:
        static constexpr std::string keyword = "constexpr";
    };
    constexpr_specifier_t constexpr_specifier;

    class cv_qualifier_t {};

    class volatile_qualifier_t : cv_qualifier_t {
    public:
        static constexpr std::string keyword = "volatile";
    };

    class const_qualifier_t : cv_qualifier_t {
    public:
        static constexpr std::string keyword = "const";
    };

    // types such as char, bool, short, int, long, double, etc., previously declared enum, class name. And even more but I don't care.
    class simple_type_specifier_t {
    public:
        simple_type_specifier_t(const std::string& id) : id_(id) {}
        const std::string& id() const { return id_; }
    private:
        std::string id_;
    };


    class specifier_t
        : public std::variant<static_specifier_t,
                              extern_specifier_t,
                              constexpr_specifier_t,
                              volatile_qualifier_t,
                              const_qualifier_t,
                              simple_type_specifier_t> {
    public:
        using variant::variant;
    };

    // Sequence of white space separated specifiers
    class decl_specifier_seq_t : public std::vector<specifier_t> {
    public:
        explicit decl_specifier_seq_t(auto ...specifiers) {
            (push_back(specifiers),...);
        }

        void add(specifier_t specifier) {
            push_back(specifier);
        }

        void remove(int i) {
            erase(begin() + i);
        }

    public:
        static constexpr auto separator = cpp::symbols::space;
    };
}

#endif /* CPP_DECLARATION_SPECIFIER_H */
