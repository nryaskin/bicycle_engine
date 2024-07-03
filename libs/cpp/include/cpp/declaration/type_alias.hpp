#pragma once
#include <string>
#include "cpp/symbols.hpp"
#include "cpp/keywords.hpp"

namespace cpp {
    class type_alias_t {
    public:
        type_alias_t(const std::string& identifier, const std::string& type_id)
            : identifier(identifier), type_id(type_id) {}

        void sequential_all(auto&& action) const {
            action(language::using_keyword);
            action(language::space);
            action(identifier);
            action(language::space);
            action(language::assignment);
            action(language::space);
            action(type_id);
            action(language::semi_collon);
        }
    private:
        std::string identifier;
        std::string type_id;
    };
};
