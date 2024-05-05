#include "type.hpp"

#include <sstream>
#include <cassert>

namespace cpp {
    std::string TypeVariant::to_string() const {
        std::stringstream ss;
        std::visit([&ss](auto&& object) {
            using T = std::decay_t<decltype(object)>;
            if constexpr (std::same_as<T, std::monostate>) {
                assert(false);
            } else {
                ss << std::format("{}", object);
            }
        }, type);

        return ss.str();
    }

}
