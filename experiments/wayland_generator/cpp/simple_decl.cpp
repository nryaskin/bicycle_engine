#include "simple_decl.hpp"

#include "format"

namespace cpp {

    SimpleDeclaration::SimpleDeclaration(Type type, const std::string& id) : type_(type), id_(id) {
    }

    std::string SimpleDeclaration::to_string() const {
        return std::format("{} {};", type_, id_);
    }
}
