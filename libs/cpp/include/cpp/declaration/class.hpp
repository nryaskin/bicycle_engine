#ifndef CPP_CLASS_H
#define CPP_CLASS_H

#if 0
#include <string>
#include <vector>
#include <variant>

#include "simple_decl.hpp"
#include "access_mod.hpp"
#include "function.hpp" 
#include "enum.hpp"
#include "container.hpp"

namespace cpp {
    using Method = Function;
    class Definition;

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

    class Class : public Container<SimpleDeclaration, AccessModifier, Method, Enum> {
    public:
        class Ctr {
        public:
            void add(Parameter p) {
                parameters.push_back(p);
            }

            const auto& params() const { return parameters; }

            friend class Definition;
            friend Document& operator<<(Document&, const Ctr&);
        private:
            std::vector<Parameter> parameters;
        };

        Class(const std::string& name);
        void add_ctr(Ctr ctr);

        friend class Definition;
        friend Document& operator<<(Document&, const Class&);
        friend Document& operator<<(Document& doc, const Definition& def);
    private:
        ClassKey class_key = ClassKey::CLASS;
        std::string class_head_name;
        std::vector<std::pair<InheritanceModel, Class>> base_clause;
        std::vector<Ctr> constructors;
    };
}
#endif
#endif /* CPP_CLASS_H */
