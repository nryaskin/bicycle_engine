#include "cpp/class.hpp"

#include "sstream"

namespace cpp {
    Class::Class(const std::string& name) : class_head_name(name) {}

    void Class::add_ctr(Ctr ctr) {
        constructors.push_back(ctr);
    }
}
