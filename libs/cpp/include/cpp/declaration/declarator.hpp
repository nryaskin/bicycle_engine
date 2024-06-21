#ifndef CPP_DECLARATION_DECLARATOR_H
#define CPP_DECLARATION_DECLARATOR_H

#include <vector>
#include <variant>
#include <string>

#include "cpp/symbols.hpp"
#include "cpp/specification.hpp"
#include "cpp/format.hpp"

namespace cpp {
    class general_id_t {
    public:
        virtual ~general_id_t() {}
    };
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
        const auto& id() const { return id_; }
        const auto& prefix() const { return prefix_; }
        void sequential_all(auto&& action) const {
            action(prefix_);
            action(language::collon);
            action(language::collon);
            action(id_);
        }
    private:
        general_id_ptr prefix_;
        unqualified_id_t id_;
    };


    // & declarator (Even if it is written on cppreference that it is any declarator it cannot be itself which means only two other options;
    class lvalue_reference_declarator_t {
    public:
        lvalue_reference_declarator_t(general_id_ptr id) : id_(id) {}

        void sequential_all(auto&& action) const {
            action(language::ampersand);
            action(language::space);
            action(id_);
        }
    private:
        general_id_ptr id_;
    };

    // && declarator 
    class rvalue_reference_declarator_t {
    public:
        rvalue_reference_declarator_t(general_id_ptr id) : id_(id) {}

        void sequential_all(auto&& action) const {
            action(language::ampersand);
            action(language::ampersand);
            action(language::space);
            action(id_);
        }
    private:
        general_id_ptr id_;
    };

    using declarator_t = std::variant<unqualified_id_t, qualified_id_t, lvalue_reference_declarator_t, rvalue_reference_declarator_t>;
}

#endif /* CPP_DECLARATION_DECLARATOR_H */
