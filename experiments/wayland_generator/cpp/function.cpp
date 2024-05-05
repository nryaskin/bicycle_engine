#include "function.hpp"

#include <sstream>

namespace cpp {
    std::string Parameter::to_string() const {
        if (default_init.size() == 0) {
            return std::format("{} {}", type.to_string(), name);
        } else {
            return std::format("{} {} {}", type.to_string(), name, default_init);
        }
    }

    std::string Function::to_string() const {
        bool comma = false; 
        std::stringstream ss;        
        ss << std::format("{}", return_type.to_string()) << " " << name << "(";
        for (const auto& parameter: parameters) {
            ss << (comma? ",": "") << parameter.to_string();
            comma = true;
        }
        ss << ");";

        return ss.str();
    }

}
