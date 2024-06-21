#pragma once

#include <string>
#include <vector>
#include "cpp/declaration/declarator.hpp"
#include "cpp/declaration/specifier.hpp"
#include "cpp/declaration/statement.hpp"
#include "cpp/declaration/init_declarator_list.hpp"

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
        void sequential_all(auto&& action) const {
            action(decl_specifier_seq);
            action(init_declarator);
        }
    private:
        decl_specifier_seq_t decl_specifier_seq;
        init_declarator_t init_declarator;
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
            : noptr_declarator(declarator),
              parameter_list(param_list),
              cv_qualifier(cv_qualifier) {}

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

    class function_t {
    public:
        explicit function_t(decl_specifier_seq_t decl_specifier_seq, 
                            function_declaration_t func_declaration,
                            compound_statement_t   compound_statement)
            : decl_specifier_seq(decl_specifier_seq),
              function_delcaration(func_declaration),
              function_body(compound_statement) {}

        void sequential_all(auto&& action) const {
            action(decl_specifier_seq);
            action(language::space);
            action(function_delcaration);
            action(function_body);
        }
    private:
        decl_specifier_seq_t   decl_specifier_seq;
        function_declaration_t function_delcaration;
        compound_statement_t   function_body;
    };
}
