#include "cpp/declaration/file.hpp"

#include <format>


namespace cpp {
    File::File(const std::string& name) : name(name){}

    void Source::add(Definition def) {
        definitions.push_back(def);
    }
}
