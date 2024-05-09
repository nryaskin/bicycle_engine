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

        friend class Definition;
        friend class Source;
        friend Document& operator<<(Document& doc, const Namespace& header);
    private:
        std::string id_ = "";
    };
};

#endif /* CPP_NAMESPACE_H */
