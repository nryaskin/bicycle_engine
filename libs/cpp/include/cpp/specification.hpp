#pragma once

#include <variant>
#include <memory>
#include <vector>

#include "cpp/symbols.hpp"
#include "cpp/keywords.hpp"

namespace language {

    class extensible_t {
    public:
        virtual ~extensible_t() {}
    };
    using extensible_ptr = std::shared_ptr<extensible_t>;

    using element_t = std::variant<keyword_t, symbol_t, extensible_ptr>;
    using extensible_list = std::vector<language::extensible_ptr>;
};
