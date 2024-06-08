#ifndef CPP_DECLARATION_INIT_DECL_LIST_H
#define CPP_DECLARATION_INIT_DECL_LIST_H

#include <vector>
#include <variant>
#include <optional>
#include <string>

#include "declarator.hpp"
#include "initializer.hpp"
#include "cpp/symbols.hpp"

namespace cpp {
    // declarator [initializer](optional, except where required, such as when initializing references or const objects)
    class init_declarator_t {
    public:
        init_declarator_t(declarator_t declarator) : declarator_(declarator) {}
        init_declarator_t(declarator_t declarator, initializer_t initializer) : declarator_(declarator), initializer_(initializer) {}

        const auto& declarator() const { return declarator_; }
        const auto& initializer() const { return initializer_; }
        static constexpr auto separator = cpp::symbols::space;
    private:
        declarator_t  declarator_;
        std::optional<initializer_t> initializer_;
    };

    // Comma separated list of one or more init-declarator
    class init_declarator_list_t {
    public:
        void add(init_declarator_t init_declarator) {
            declarators.push_back(init_declarator);
        }
    static constexpr std::string separator = cpp::symbols::comma;
    const auto& init_declarators() const { return declarators; }
    private:
        std::vector<init_declarator_t> declarators;
    };
};

#endif /* CPP_DECLARATION_INIT_DECL_LIST_H */
