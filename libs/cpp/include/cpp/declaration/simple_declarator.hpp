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
        static constexpr std::string close = cpp::symbols::semi_collon;
        static constexpr std::string separator = cpp::symbols::space;

        const auto& decl_spec_seq() const { return specifier_seq; }
        const auto& init_decl_list() const { return init_declarator_list; }

    private:
        decl_specifier_seq_t   specifier_seq;
        init_declarator_list_t init_declarator_list;
    };
};

#endif /* CPP_DECLARATION_SIMPLE_DECLARATOR_H */
