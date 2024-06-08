#ifndef CPP_TYPE_H
#define CPP_TYPE_H
#include <string>
#include <set>
#include <format>

#include "includes.hpp"

namespace cpp {
    class Document;

    //TODO: If I ever want to continue work on this make links between definitions and types.
    class Type {
    public:
        Type(const std::string& id) : id_(id) {}

        std::string id() const { return id_; }
    private:
        std::string id_;
    };

    class Ref {
    public:
        Ref(Type type) : type(type) {}

        friend Document& operator<<(Document&, const Ref&);
    private:
        Type type;
    };

    class RRef {
    public:
        RRef(Type type) : type(type) {}

        friend Document& operator<<(Document&, const RRef&);
    private:
        Type type;
    };

    class TypeVariant {
    public:
        using var_t = std::variant<std::monostate, Type, Ref, RRef>;

        template<typename T>
        TypeVariant(T t) : type(t) {}

        friend Document& operator<<(Document&, const TypeVariant&);
    private:
        var_t type;
    };

    using type_t = TypeVariant;
}

#endif /* CPP_TYPE_H */
