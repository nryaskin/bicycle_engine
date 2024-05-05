#include "simple_decl.hpp"

#include "format"

namespace cpp {

    SimpleDeclaration::SimpleDeclaration(type_t type, const std::string& id) : type_(type), id_(id) {
    }

    SimpleDeclaration::SimpleDeclaration(type_t type, const std::string& id, const std::string& initializer) :
        SimpleDeclaration(type, id) {
        initializer_ = initializer;
    }

    std::string SimpleDeclaration::to_string() const {
        if (initializer_.size() == 0) {
            return std::format("{} {};", type_.to_string(), id_);
        }
        else {
            return std::format("{} {} {};", type_.to_string(), id_, initializer_);
        }
    }
}
