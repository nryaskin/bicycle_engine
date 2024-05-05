#ifndef CPP_DEFINITION_H
#define CPP_DEFINITION_H

#include <vector>
#include <string>
#include <sstream>
#include <format>

#include "class.hpp"
#include "namespace.hpp"

namespace cpp {
    // Warnign: I don't care about anything cpp related here other then what I need for waylad so I am going to create only definition of class methods here.
    // So body here is absolutely simple string lines.
    class MethodBody {
    public:
        void add(const std::string& line) {
            lines.push_back(line);
        }

        std::string to_string() const {
            std::stringstream ss;

            for(const auto& line : lines) {
                ss << line << "\n";
            }

            return ss.str();
        }
    private:
        std::vector<std::string> lines;
    };

    class Definition {
    public:
        Definition(Class cl,
                   Class::Ctr ctr,
                   MethodBody method_body) :
            class_(cl),
            ctr_(ctr),
            method_body_(method_body)  {}

        Definition(Class cl,
                   Method method,
                   MethodBody method_body) :
            class_(cl),
            method_(method),
            method_body_(method_body)  {}

        std::string to_string() const {
            std::stringstream ss;
            bool comma = false;

            // NOTE: Very dirty, just need it to work.
            if (method_.has_value()) {
                ss << std::format("{} {}::{}(", method_.value().return_type.to_string(), class_.class_head_name, method_.value().name);
                for (const auto& parameter: method_.value().parameters) {
                    ss << (comma? ",": "") << parameter.to_string();
                    comma = true;
                }
            } else {
                // Constructor
                ss << std::format("{}::{}(", class_.class_head_name, class_.class_head_name);
                for (const auto& parameter: ctr_->parameters) {
                    ss << (comma? ",": "") << parameter.to_string();
                    comma = true;
                }

            }

            ss << std::format(") {{\n{}}}", method_body_.to_string());
            return ss.str();
        }
    private:
        Class                     class_;
        std::optional<Class::Ctr> ctr_;
        std::optional<Method>     method_;
        MethodBody                method_body_;
    };

};

#endif /* CPP_DEFINITION_H */
