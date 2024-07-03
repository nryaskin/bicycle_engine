#ifndef CPP_DECLARATION_INIT_DECL_LIST_H
#define CPP_DECLARATION_INIT_DECL_LIST_H

#include <vector>
#include <variant>
#include <optional>
#include <string>

#include "declarator.hpp"
#include "initializer.hpp"
#include "cpp/symbols.hpp"
#include "cpp/format.hpp"

namespace cpp {
    // declarator [initializer](optional, except where required, such as when initializing references or const objects)
    class init_declarator_t {
    public:
        init_declarator_t(auto declarator) : declarator_(declarator) {}
        init_declarator_t(declarator_t declarator, initializer_t initializer) : declarator_(declarator), initializer_(initializer) {}
        init_declarator_t(const init_declarator_t& init_decl) {
            *this = init_decl;
        }

        init_declarator_t& operator=(const init_declarator_t& init_decl) {
            declarator_ = init_decl.declarator_;
            initializer_ = init_decl.initializer_;
            return *this;
        }

        const auto& declarator() const { return declarator_; }
        const auto& initializer() const { return initializer_; }

        void sequential_all(auto&& action) const {
            action(declarator_);
            if (initializer().has_value()) {
                action(language::space);
                action(initializer().value());
            }
        }
    private:
        declarator_t  declarator_;
        std::optional<initializer_t> initializer_;
    };

    // Comma separated list of one or more init-declarator
    class init_declarator_list_t : public std::vector<init_declarator_t> {
    public:
        using format = language::format_t<init_declarator_t, language::zero_or_more<language::comma_t, init_declarator_t>>;
        explicit init_declarator_list_t(init_declarator_t init_declarator) { push_back(init_declarator); }
        void add(init_declarator_t init_declarator) {
            push_back(init_declarator);
        }

        void sequential_all(auto&& action) const {
            for(auto& id : *this) {
                action(id);
            }
        }
    };
};

#endif /* CPP_DECLARATION_INIT_DECL_LIST_H */
