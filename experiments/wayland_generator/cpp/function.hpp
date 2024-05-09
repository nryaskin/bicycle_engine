#ifndef CPP_FUNCTION_H
#define CPP_FUNCTION_H
#include <string>
#include <vector>

#include "type.hpp"


namespace cpp {
    class Definition;

    class Parameter {
    public:
        Parameter(type_t type, std::string name = "", std::string default_init = "") :
            type(type), name(name), default_init(default_init) {}

        friend Document& operator<<(Document& doc, const Parameter& ns);
    private:
        type_t type;
        std::string name;
        std::string default_init;
    };

    class Function {
    public:
        template<typename ...Param>
        Function(std::string name, type_t return_type, Param... param)
        : Function(name, return_type) {
            (parameters.push_back(param),...);
        }

        Function(std::string name, type_t return_type) : name(name), return_type(return_type) {}
        void add_parameter(Parameter parameter) {
            parameters.push_back(parameter);
        }

        friend class Definition;
        friend Document& operator<<(Document& doc, const Function& ns);
        friend Document& operator<<(Document& doc, const Definition& def);
    private:
        std::string name;
        type_t return_type;
        std::vector<Parameter> parameters;
    };
}

#endif /* CPP_FUNCTION_H */
