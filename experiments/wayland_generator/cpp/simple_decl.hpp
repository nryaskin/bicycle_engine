#ifndef CPP_SIMPLE_DECL_H
#define CPP_SIMPLE_DECL_H

#include <string>

#include "type.hpp"

namespace cpp {
    class SimpleDeclaration {
    public:
        SimpleDeclaration(type_t, const std::string&);
        SimpleDeclaration(type_t type, const std::string& id, const std::string& initializer);

        std::string to_string() const;

    private:
        type_t      type_;
        std::string id_;
        std::string initializer_;
    };
}

#endif /* CPP_SIMPLE_DECL_H */
