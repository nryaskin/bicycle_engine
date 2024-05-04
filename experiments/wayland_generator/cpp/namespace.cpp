#include "namespace.hpp"

#include <format>

namespace cpp {

    std::string Namespace::to_string() const {
        return std::format("{} {} {{\n{}\n}}", Namespace::keyword, id_, get_body()); 
    }


    std::string Namespace::get_body() const {
        std::string body;
        for (const auto& decl: container) {
            std::visit([&body](auto&& object) {
                using T = std::decay_t<decltype(object)>;
                if constexpr (!std::same_as<T, std::monostate>) { 
                    body.append(object.to_string());
                }
            }, decl);
        }

        return body;
    }
}
