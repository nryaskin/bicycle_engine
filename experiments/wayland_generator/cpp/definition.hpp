#ifndef CPP_DEFINITION_H
#define CPP_DEFINITION_H

#include <vector>
#include <string>
#include <sstream>
#include <format>

#include "class.hpp"
#include "namespace.hpp"

namespace cpp {
    class Document;
    // Warnign: I don't care about anything cpp related here other then what I need for waylad so I am going to create only definition of class methods here.
    // So body here is absolutely simple string lines.
    class MethodBody {
    public:
        void add(const std::string& line) {
            lines.push_back(line);
        }


        friend Document& operator<<(Document& doc, const MethodBody& def);

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

        friend Document& operator<<(Document& doc, const Definition& def);

    private:
        Class                     class_;
        std::optional<Class::Ctr> ctr_;
        std::optional<Method>     method_;
        MethodBody                method_body_;
    };

};

#endif /* CPP_DEFINITION_H */
