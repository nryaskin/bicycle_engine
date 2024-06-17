#ifndef CPP_DECLARATION_DECLARATOR_H
#define CPP_DECLARATION_DECLARATOR_H

#include <vector>
#include <variant>
#include <string>

#include "cpp/symbols.hpp"
#include "cpp/specification.hpp"
#include "cpp/format.hpp"

namespace cpp {
    class general_id_t : public language::extensible_t {};
    using general_id_ptr = std::shared_ptr<general_id_t>;

    // just name that is declared
    class unqualified_id_t : public general_id_t {
    public:
        unqualified_id_t(const std::string& id) : id_(id) {}
        static auto make(const std::string& id) { return std::make_shared<unqualified_id_t>(id); }
        const auto& id() const { return id_; }
    private:
        std::string id_;
    };


    // Declaration that uses qualified identifier
    class qualified_id_t : public general_id_t {
    public:
        static constexpr auto collons = language::collon_t::symbol + language::collon_t::symbol;
        const auto& id() const { return id_; }
        const auto& prefix() const { return prefix_; }
    private:
        general_id_ptr prefix_;
        unqualified_id_t id_;
    };


    // & declarator (Even if it is written on cppreference that it is any declarator it cannot be itself which means only two other options;
    class lvalue_reference_declarator_t : public language::extensible_list {
    public:
        using format = language::format_t<language::ampersand_t, language::space_t, general_id_t>;
        lvalue_reference_declarator_t(general_id_ptr id) { push_back(id); }
    };

    // && declarator 
    class rvalue_reference_declarator_t : public language::extensible_list {
    public:
        using format = language::format_t<language::ampersand_t, language::ampersand_t, language::space_t, general_id_t>;
        rvalue_reference_declarator_t(general_id_ptr id) { push_back(id); }
    };

    using declarator_t = std::variant<unqualified_id_t, qualified_id_t, lvalue_reference_declarator_t, rvalue_reference_declarator_t>;
}

#endif /* CPP_DECLARATION_DECLARATOR_H */
