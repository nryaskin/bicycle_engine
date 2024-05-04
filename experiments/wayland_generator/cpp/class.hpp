#ifndef CPP_CLASS_H
#define CPP_CLASS_H

#include <string>
#include <vector>
#include <variant>

#include "simple_decl.hpp"
#include "access_mod.hpp"
#include "function.hpp" 
#include "container.hpp"

namespace cpp {
    using Method = Function;

    enum class InheritanceModel {
        PUBLIC,
        PROTECTED,
        PRIVATE
    };

    constexpr std::string inheritance_to_string(InheritanceModel im) {
        switch (im) {
            case InheritanceModel::PRIVATE:
                return "private";
            case InheritanceModel::PUBLIC:
                return "public";
            case InheritanceModel::PROTECTED:
                return "protected";
        }
        return "";
    }

    enum class ClassKey {
        CLASS,
        STRUCT,
        UNION
    };

    class Class : public Container<SimpleDeclaration, AccessModifier, Method> {
    public:
        Class(const std::string& name);

        ClassKey class_key = ClassKey::CLASS;
        std::string class_head_name;
        std::vector<std::pair<InheritanceModel, Class>> base_clause;

        std::string to_string() const;
    };
}
#endif /* CPP_CLASS_H */
