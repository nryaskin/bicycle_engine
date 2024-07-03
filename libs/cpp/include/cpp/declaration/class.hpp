#pragma once

#include <variant>
#include <string>
#include <vector>
#include <optional>

#include "cpp/declaration/declarator.hpp"
#include "cpp/declaration/function.hpp"
#include "cpp/symbols.hpp"
#include "cpp/declaration/enum.hpp"
#include "cpp/declaration/type_alias.hpp"

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

    class access_specifier_t {
    public:
        enum class access_t {
            PUBLIC,
            PRIVATE,
            PROTECTED
        };

        constexpr access_specifier_t(access_t access) : access(access) {}

        void sequential_all(auto&& action) const {
            switch(access) {
                case access_t::PUBLIC:
                    action(language::public_keyword);
                    break;
                case access_t::PROTECTED:
                    action(language::protected_keyword);
                    break;
                case access_t::PRIVATE:
                    action(language::private_keyword);
                    break;
            }
            action(language::collon);
        }
    private:
        access_t access;
    };
    static constexpr auto public_access    = access_specifier_t(access_specifier_t::access_t::PUBLIC);
    static constexpr auto protected_access = access_specifier_t(access_specifier_t::access_t::PROTECTED);
    static constexpr auto private_access   = access_specifier_t(access_specifier_t::access_t::PRIVATE);

    class base_clause_element_t {
    public:
        base_clause_element_t(general_id_ptr id, inheritance_model_t inheritance_model = inheritance_model_t::PUBLIC)
            : id_(id),
              inheritance_model_(inheritance_model) {}

        const auto& inheritance_model() const { return inheritance_model_; }
        const auto& id() const { return id_; }
        void sequential_all(auto&& action) const {
            switch(inheritance_model_) {
                case inheritance_model_t::PUBLIC:
                    action(language::public_keyword);
                    break;
                case inheritance_model_t::PROTECTED:
                    action(language::protected_keyword);
                    break;
                case inheritance_model_t::PRIVATE:
                    action(language::private_keyword);
                    break;
            }
            action(id());
        }
    private:
        inheritance_model_t inheritance_model_;
        general_id_ptr id_;
    };

    class base_clause_t : public std::vector<base_clause_element_t> {
    public:
        void sequential_all(auto&& action) const {
            auto it = begin();
            while(++it != end()) {
                action(language::comma);
                action(language::space);
                action(*it);
            }
        }
    };

    using member_object_t = cpp::simple_declaration_t;
    using member_function_t = cpp::function_t;

    using member_specification_element_t = std::variant<access_specifier_t,
                                                        type_alias_t,
                                                        member_object_t,
                                                        member_function_t,
                                                        language::comment_t,
                                                        enum_specifier_t>;

    class member_specification_t : public std::vector<member_specification_element_t> {
    public:
        void sequential_all(auto&& action) const {
            for(auto mse : *this) {
                action(mse);
                action(language::newline);
            }
        }
    };

    class clas {
    public:
        clas(const std::string& class_head_name,
             class_key_t class_key = class_key_t::CLASS)
            : class_head_name_(class_head_name),
              class_key_(class_key) {}

        void set_base_clause(const base_clause_t& base_clause) {
            base_clause_ = base_clause;
        }

        void set_member_specification(const member_specification_t& ms) {
            member_specification_ = ms;
        }

        const auto& class_head_name() const { return class_head_name_; }
        auto class_key() const { return class_key_; }

        static void class_key_apply(class_key_t class_key, auto&& action) {
            switch(class_key) {
                case class_key_t::CLASS:
                    action(language::class_keyword);
                    break;
                case class_key_t::STRUCT:
                    action(language::struct_keyword);
                    break;
                case class_key_t::UNION:
                    action(language::union_keyword);
                    break;
            }
        }

        void append(const member_specification_element_t& ms) {
            if (!member_specification_) {
                member_specification_ = cpp::member_specification_t {};
            }
            member_specification_.value().push_back(ms);
        }

        void sequential_all(auto&& action) const {
            class_key_apply(class_key(), action);
            action(language::space);
            action(class_head_name_);
            if (base_clause_) {
                action(language::space);
                action(base_clause_.value());
            }
            action(language::space);
            action(language::open_curly_brace);
            action(language::newline);
            if (member_specification_) {
                action(member_specification_.value());
            }
            action(language::close_curly_brace);
            action(language::semi_collon);
        }
    private:
        std::string                           class_head_name_;
        class_key_t                           class_key_;
        std::optional<base_clause_t>          base_clause_;
        std::optional<member_specification_t> member_specification_;
    };

    class forward_declaration {
    public:
        void sequential_all(auto&& action) const {
            clas::class_key_apply(class_key, action);
            action(language::space);
            action(class_head_name);
            action(language::semi_collon);
        }
    private:
        class_key_t   class_key;
        std::string   class_head_name;
    };
}
