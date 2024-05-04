#include "function.hpp"

#include <sstream>

namespace cpp {
    std::string Parameter::to_string() const {
        return std::format("{} {}", type, name);
    }

    std::string Function::to_string() const {
        bool comma = false; 
        std::stringstream ss;        
        ss << std::format("{}", return_type) << " " << name << "(";
        for (const auto& parameter: parameters) {
            ss << (comma? ",": "") << parameter.to_string();
            comma = true;
        }
        ss << ");";

        return ss.str();
    }

}
