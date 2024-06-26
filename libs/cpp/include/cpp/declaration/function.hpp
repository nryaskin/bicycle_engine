#pragma once

#include <string>
#include <vector>
#include "cpp/declaration/declarator.hpp"
#include "cpp/declaration/specifier.hpp"
#include "cpp/declaration/statement.hpp"
#include "cpp/declaration/init_declarator_list.hpp"
#include "cpp/comments.hpp"

namespace cpp {
    // Function definition:
    // decl-specifier-seq(optional) declarator virt-specifier-seq(optional) function-body
    // funciton-body is one of the following:
    // ctor-initializer(optional) compound-statement (1)
    // function-try-block                            (2)
    // = delete;                                     (3)
    // = default;                                    (5)
    // 1) Regular function body <- this is then only defintion of interest to me.
    // 
    // decl-specifier-seq - the return type with specifier, as in the declaration grammar
    // declarator - function declarator, same as in the function declaration grammar
    // virt-specifier-seq - override, final, or their combination in any order
    // ctor-initializer - member initializer list, only allowed in constructors
    // compound-statement - the brace-enclosed sequence of statements that constitutes the body of function.
    //
    // declarator:
    // noptr-declarator ( parameter-list ) cv
    // parameter-list - possibly empty comma-separarted list of function parameters
    class parameter_t {
    public:
        explicit parameter_t(decl_specifier_seq_t decl_spec_sec, init_declarator_t init_decl)
            : decl_specifier_seq(decl_spec_sec),
              init_declarator(init_decl) {}
        auto init_decl() const { return init_declarator; }
        void sequential_all(auto&& action) const {
            action(decl_specifier_seq);
            action(language::space);
            action(init_declarator);
        }
    private:
        decl_specifier_seq_t decl_specifier_seq;
        init_declarator_t    init_declarator;
    };

    class parameter_list_t : public std::vector<parameter_t> {
    public:
        void sequential_all(auto&& action) const {
            if (size() == 0)
                return;

            auto it = begin();
            action(*it);
            while(++it != end()) {
                action(language::comma);
                action(language::space);
                action(*it);
            }
        }
    };

    class function_declaration_t {
    public:
        explicit function_declaration_t(declarator_t declarator,
                                        parameter_list_t param_list,
                                        cv_qualifier_t   cv_qualifier = {})
            : noptr_declarator(std::move(declarator)),
              parameter_list(param_list),
              cv_qualifier(cv_qualifier) {}
        auto noptr_decl() const { return noptr_declarator; }
        auto param_list() const { return parameter_list; }

        void sequential_all(auto&& action) const {
            action(noptr_declarator);
            action(language::open_brace);
            action(parameter_list);
            action(language::close_brace);
            action(language::space);
            action(cv_qualifier);
        }
    private:
        declarator_t     noptr_declarator;
        parameter_list_t parameter_list; 
        cv_qualifier_t   cv_qualifier;
    };

    class ctor_initializer_t {
    public:
        class element_t {
        public:
            element_t(unqid_t id) : id(id) { }
            element_t(unqid_t id, expression_t expr) : id(id) {
                expression_list.push_back(expr);
            }

            expression_list_t expression_list;
            unqid_t id;
            void sequential_all(auto&& action) const {
                action(id);
                action(language::open_brace);
                action(language::space);
                if (expression_list.size() > 0) {
                    action(expression_list);
                    action(language::space);
                }
                action(language::close_brace);
            }
        };

        void sequential_all(auto&& action) const {
            action(language::collon);
            action(language::space);
            auto it = elements_.begin();
            action(*it);
            while(++it != elements_.end()) {
                action(language::comma);
                action(language::space);
                action(*it);
            }
        }

        const auto& elements() const { return elements_; }
        auto& elements() { return elements_; }

    private:
        std::vector<element_t> elements_;
    };

    class function_body_t {
    public:
        function_body_t(compound_statement_t compound_statement) : compound_statement(compound_statement) {}
        function_body_t(compound_statement_t compound_statement, ctor_initializer_t ctor_init)
            : compound_statement(compound_statement),
              ctor_init(ctor_init) {}
        void sequential_all(auto&& action) const {
            if (ctor_init) {
                action(ctor_init.value());
            }
            action(language::space);
            action(compound_statement);
        }

    private:
        std::optional<ctor_initializer_t> ctor_init;
        compound_statement_t              compound_statement;
    };

    class function_t {
    public:
        explicit function_t(function_declaration_t declaration, function_body_t body)
            : declaration(declaration), body(body) {}
        explicit function_t(decl_specifier_seq_t decl_specifier_seq, 
                            function_declaration_t declaration,
                            function_body_t        body)
            : decl_specifier_seq(decl_specifier_seq),
              declaration(declaration),
              body(body) {}

        void sequential_all(auto&& action) const {
            if (decl_specifier_seq) {
                action(decl_specifier_seq.value());
                action(language::space);
            }
            action(declaration);
            action(language::space);
            action(body);
        }
    private:
        std::optional<decl_specifier_seq_t> decl_specifier_seq;
        function_declaration_t              declaration;
        function_body_t                     body;
    };
}
