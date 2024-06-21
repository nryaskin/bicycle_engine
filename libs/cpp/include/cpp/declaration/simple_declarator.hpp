#ifndef CPP_DECLARATION_SIMPLE_DECLARATOR_H
#define CPP_DECLARATION_SIMPLE_DECLARATOR_H

#include "specifier.hpp"
#include "init_declarator_list.hpp"
#include "cpp/symbols.hpp"

namespace cpp {
    // decl-specifier-seq init-declarator-list(optional);
    class simple_declaration_t {
    public:
        simple_declaration_t(decl_specifier_seq_t spec_seq, init_declarator_list_t init_decl_list) : specifier_seq(spec_seq), init_declarator_list(init_decl_list) {}
        static constexpr auto trailer = language::semi_collon_t {};
        static constexpr auto separator = language::space_t {};

        const auto& decl_spec_seq() const { return specifier_seq; }
        const auto& init_decl_list() const { return init_declarator_list; }
        void sequential_all(auto&& action) const {
            action(specifier_seq);
            action(language::space);
            action(init_declarator_list);
            action(language::semi_collon);
        }

    private:
        decl_specifier_seq_t   specifier_seq;
        init_declarator_list_t init_declarator_list;
    };
};

#endif /* CPP_DECLARATION_SIMPLE_DECLARATOR_H */
