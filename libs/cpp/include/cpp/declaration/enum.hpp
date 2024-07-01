#ifndef CPP_ENUM_H
#define CPP_ENUM_H

#include <string>
#include <vector>
#include "cpp/keywords.hpp"
#include "cpp/symbols.hpp"
#include "cpp/declaration/specifier.hpp"
#include "cpp/declaration/declarator.hpp"

namespace cpp {
    class enum_specifier_t {
    public:
        class entity_t {
        public:
            entity_t(auto&& name, const std::string& value) : name(name), value(value) {}

            friend class enum_specifier_t;
        private:
            unqid_t name;
            std::string value;
        };

        explicit enum_specifier_t(const std::string& name) : name(name) {}
        explicit enum_specifier_t(const std::string& name, auto&& base)
            : name(name), base(base) {}

        void add(auto&& e) {
            entities.push_back(e);
        }

        void sequential_all(auto&& action) const {
            action(language::enum_keyword);
            action(language::space);
            action(language::class_keyword);
            action(language::space);
            action(name);
            if (base) {
                action(language::space);
                action(language::collon);
                action(language::space);
                action(base.value());
            }
            action(language::space);
            action(language::open_curly_brace);
            action(language::newline);
            for (auto& entity : entities) {
                action(entity.name);
                action(language::space);
                action(language::assignment);
                action(language::space);
                action(entity.value);
                action(language::comma);
                action(language::newline);
            }
            action(language::close_curly_brace);
            action(language::semi_collon);
        }

    private:
        unqid_t                                name;
        std::optional<simple_type_specifier_t> base;
        std::vector<entity_t>                  entities;
    };
}

#endif /* CPP_ENUM_H */
