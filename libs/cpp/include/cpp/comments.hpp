#pragma once

#include <string>
#include "cpp/symbols.hpp"

namespace language {
    class comment_t : public std::string {
    public:
        void sequential_all(auto&& action) const {
            action(language::solidus);
            action(language::solidus);
            action(language::space);
            const std::string& str = *this;
            action(str);
        }
    };
}
