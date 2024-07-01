#ifndef CPP_NAMESPACE_H
#define CPP_NAMESPACE_H

#include <string>
#include <vector>
#include "cpp/keywords.hpp"
#include "cpp/declaration/simple_declarator.hpp"
#include "cpp/declaration/function.hpp"
#include "cpp/declaration/class.hpp"

namespace cpp {

    class namespace_t;
    using ns_body = std::variant<simple_declaration_t, function_t, clas, namespace_t>;

    class namespace_t : public std::vector<ns_body> {
    public:
        namespace_t(const std::string& ns_name) : ns_name(ns_name) {}
        void sequential_all(auto&& action) const {
            action(language::namespace_keyword);
            action(language::space);
            action(ns_name);
            action(language::space);
            action(language::open_curly_brace);
            action(language::newline);
            for (auto& decl : *this) {
                action(decl);
                action(language::newline);
            }
            action(language::close_curly_brace);
        }
    private:
        std::string ns_name;
    };

};

#endif /* CPP_NAMESPACE_H */
