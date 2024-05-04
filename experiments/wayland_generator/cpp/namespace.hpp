#ifndef CPP_NAMESPACE_H
#define CPP_NAMESPACE_H

#include <string>
#include <vector>
#include "simple_decl.hpp"
#include "function.hpp"
#include "class.hpp"
#include "container.hpp"

namespace cpp {

    class Namespace : public Container<SimpleDeclaration, Function, Class, Namespace> {
    public:
        static constexpr std::string keyword = "namespace";
        Namespace() = default;
        Namespace(const std::string& id) : id_(id) {}
        Namespace(const Namespace&) = default;
        Namespace& operator=(const Namespace&) = default;

        std::string to_string() const;

    private:
        std::string get_body() const;
        std::string id_ = "";
    };
};

#endif /* CPP_NAMESPACE_H */
