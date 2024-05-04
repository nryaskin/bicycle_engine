#include "class.hpp"

#include "sstream"

namespace cpp {
    Class::Class(const std::string& name) : class_head_name(name) {}

    std::string Class::to_string() const {
        std::stringstream ss;

        std::string sk;
        if (class_key == ClassKey::CLASS) {
            sk = "class";
        } else if (class_key == ClassKey::STRUCT) {
            sk = "struct";
        } else if (class_key == ClassKey::UNION) {
            sk = "union";
        }

        ss << sk << " " << class_head_name << " ";
        if (base_clause.size() > 0) {
            ss << ": ";
            bool comma = false;
            for (const auto& base : base_clause) {
                ss << (comma? ",": "") << inheritance_to_string(base.first) << " " << base.second.class_head_name;
                comma = true;
            }
        }
        ss << "{\n";

        for (const auto& decl: container) {
            std::visit([&ss](auto&& object) {
                using T = std::decay_t<decltype(object)>;
                if constexpr (!std::same_as<T, std::monostate>) { 
                    ss << object.to_string() << "\n";
                }
            }, decl);
        }
        ss << "};\n";

        return ss.str();
    }
}
