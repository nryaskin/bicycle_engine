#ifndef CPP_FUNCTION_H
#define CPP_FUNCTION_H
#include <string>
#include <vector>

#include "type.hpp"


namespace cpp {
    class Parameter {
    public:
        Parameter(Type type, std::string name = "") : type(type), name(name) {}

        // TODO: I think can replace all of this with format or something like that to use with vformat at least.
        std::string to_string() const;
    private:
        Type type;
        std::string name;
    };

    class Function {
    public:
        template<typename ...Param>
        Function(std::string name, Type return_type, Param... param)
        : Function(name, return_type) {
            (parameters.push_back(param),...);
        }

        Function(std::string name, Type return_type) : name(name), return_type(return_type) {}
        void add_parameter(Parameter parameter) {
            parameters.push_back(parameter);
        }

        std::string to_string() const;

    private:
        std::string name;
        Type return_type;
        std::vector<Parameter> parameters;
    };
}

#endif /* CPP_FUNCTION_H */
