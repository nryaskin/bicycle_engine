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
            type(type), name_(name), default_init(default_init) {}

        const std::string& name() const { return name_; }

        friend Document& operator<<(Document& doc, const Parameter& ns);
    private:
        type_t type;
        std::string name_;
        std::string default_init;
    };

    class Function {
    public:
        template<typename ...Param>
        Function(std::string name, type_t return_type, Param... param)
        : Function(name, return_type) {
            (parameters.push_back(param),...);
        }

        Function(std::string name, type_t return_type) : name_(name), return_type(return_type) {}
        void add_parameter(Parameter parameter) {
            parameters.push_back(parameter);
        }

        void append_parameters(const std::vector<Parameter>& params) {
            parameters.insert(parameters.end(), params.begin(), params.end());
        }

        const std::vector<Parameter>& get_params() const { return parameters; }
        const std::string& name() const { return name_; }

        friend class Definition;
        friend Document& operator<<(Document& doc, const Function& ns);
        friend Document& operator<<(Document& doc, const Definition& def);
    private:
        std::string name_;
        type_t return_type;
        std::vector<Parameter> parameters;
    };
}

#endif /* CPP_FUNCTION_H */
