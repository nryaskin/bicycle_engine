#ifndef CPP_DECLARATION_DECLARATOR_H
#define CPP_DECLARATION_DECLARATOR_H

#include <vector>
#include <variant>
#include <string>

#include "cpp/symbols.hpp"

namespace cpp {
    // id
    class unqualified_id_t {
    public:
        unqualified_id_t(const std::string& id) : id_(id) {}
        const std::string& id() const { return id_; }
    private:
        std::string id_;
    };

    // prefix(which is any of namespace, class, enum... names) ::id
    class qualified_id_t {
    public:
        static constexpr std::string collons = cpp::symbols::collon + cpp::symbols::collon;
        const std::string& id() const { return id_; }
        const std::vector<unqualified_id_t> prefix() const { return prefix_; }
    private:
        std::vector<unqualified_id_t> prefix_;
        std::string id_;
    };

    using general_id_t = std::variant<unqualified_id_t, qualified_id_t>;

    // & declarator (Even if it is written on cppreference that it is any declarator it cannot be itself which means only two other options;
    class lvalue_reference_declarator_t {
    public:
        lvalue_reference_declarator_t(general_id_t id) : declarator_(id) {}
        static constexpr std::string ref = cpp::symbols::ampersand;
        const general_id_t& declarator() const { return declarator_; }
    private:
         general_id_t declarator_;
    };

    // && declarator 
    class rvalue_reference_declarator_t {
    public:
        rvalue_reference_declarator_t(general_id_t id) : declarator_(id) {}
        static constexpr std::string rref = cpp::symbols::ampersand + cpp::symbols::ampersand;
        const general_id_t& declarator() const { return declarator_; }
    private:
        general_id_t declarator_;
    };

    class declarator_t :
        public std::variant<unqualified_id_t, qualified_id_t, lvalue_reference_declarator_t, rvalue_reference_declarator_t> {
            using variant::variant;
    };
}

#endif /* CPP_DECLARATION_DECLARATOR_H */
