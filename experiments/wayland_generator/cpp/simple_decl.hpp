#ifndef CPP_SIMPLE_DECL_H
#define CPP_SIMPLE_DECL_H

#include <string>

#include "type.hpp"

namespace cpp {
    class SimpleDeclaration {
    public:
        SimpleDeclaration(Type, const std::string&);

        std::string to_string() const;

    private:
        Type        type_;
        std::string id_;
    };
}

#endif /* CPP_SIMPLE_DECL_H */
