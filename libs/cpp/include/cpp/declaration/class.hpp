#pragma once

#include <variant>
#include <string>
#include <vector>
#include <optional>

#include "cpp/declaration/declarator.hpp"
#include "cpp/symbols.hpp"

// class-key class-head-name final(optional) base-clause(optional) { member-specification }
// class-key - class | struct | union
// class-head-name - the name of the class optionally qualified
// final - if present cannot be derrived 
// base-clause - list of one or more base classes and the model of inheritance used for each.
// member-specification - list of access specifiers, member object and member function declaration and definition.

// forward declaration
// class-key identifier;

// Member specification
// 1. Member declaration:
// decl-specifier-seq (optional) member-declarator-list(optional) ;
// member-declarator-list - similar to an init-declarator-list, doesn't allow direct non list intialization syntax.
// 2. Function definitions:
// 3. Access specifiers

// Constructor:
// class-name ( parameter-list ) member-initializer-list 

namespace cpp {
    enum class class_key_t {
        CLASS,
        STRUCT,
        UNION
    };

    enum class inheritance_model_t {
        PUBLIC,
        PRIVATE,
        PROTECTED
    };

    enum class access_specifier_t {
        PUBLIC,
        PRIVATE,
        PROTECTED
    };

    class base_clause_element_t {
    public:
        base_clause_element_t(general_id_t id, inheritance_model_t inheritance_model = inheritance_model_t::PUBLIC)
            : id_(id),
              inheritance_model_(inheritance_model) {}

        const auto& inheritance_model() const { return inheritance_model_; }
        const auto& id() const { return id_; }
    private:
        inheritance_model_t inheritance_model_;
        general_id_t id_;
    };

    class base_clause_t : public std::vector<base_clause_element_t> {
    public:
        static constexpr auto separator = cpp::symbols::comma;
    };

    class member_object_t;
    class member_function_t;

    using member_specification_var_t = std::variant<access_specifier_t, member_object_t, member_function_t>;

    class member_specification_t : public std::vector<member_specification_t> {
    public:
        static constexpr auto separator = cpp::symbols::newline;
    };

    class clas {
    public:
        static constexpr std::string open   = cpp::symbols::open_curly_brace;
        static constexpr std::string close  = cpp::symbols::close_curly_brace;
        clas(const std::string& class_head_name, class_key_t class_key = class_key_t::CLASS)
            : class_head_name_(class_head_name),
              class_key_(class_key) {}
        const auto& class_head_name() const { return class_head_name_; }
        auto class_key() const { return class_key_; }
    private:
        std::string                           class_head_name_;
        class_key_t                           class_key_;
        std::optional<base_clause_t>          base_clause_;
        std::optional<member_specification_t> member_specification_;
    };

    class forward_declaration {
        static constexpr std::string trailer = cpp::symbols::semi_collon;
    public:
        class_key_t   class_key;
        std::string   class_head_name;
    };
}
