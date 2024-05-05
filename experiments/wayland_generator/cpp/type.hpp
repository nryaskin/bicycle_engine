#ifndef CPP_TYPE_H
#define CPP_TYPE_H
#include <string>
#include <set>
#include <format>

#include "incl.hpp"

namespace cpp {
    //TODO: If I ever want to continue work on this make links between definitions and types.
    class Type {
    public:
        Type(const std::string& id) : id_(id) {}
        std::string id_;
    };

    class Ref {
    public:
        Ref(Type type) : type(type) {}

        friend struct std::formatter<cpp::Ref>;
    private:
        Type type;
    };

    class RRef {
    public:
        RRef(Type type) : type(type) {}

        friend struct std::formatter<cpp::RRef>;
    private:
        Type type;
    };

    class TypeVariant {
    public:
        using var_t = std::variant<std::monostate, Type, Ref, RRef>;
        template<typename T>
        TypeVariant(T t) : type(t) {}

        std::string to_string() const;
    private:
        var_t type;
    };

    using type_t = TypeVariant;
}

template<>
struct std::formatter<cpp::Type> {
    constexpr auto parse(std::format_parse_context& ctx) {
        return ctx.begin();
    }

    auto format(const cpp::Type& obj, std::format_context& ctx) const {
        return std::format_to(ctx.out(), "{}", obj.id_);
    }
};

template<>
struct std::formatter<cpp::Ref> {
    constexpr auto parse(std::format_parse_context& ctx) {
        return ctx.begin();
    }

    auto format(const cpp::Ref& obj, std::format_context& ctx) const {
        return std::format_to(ctx.out(), "{}&", obj.type);
    }
};

template<>
struct std::formatter<cpp::RRef> {
    constexpr auto parse(std::format_parse_context& ctx) {
        return ctx.begin();
    }

    auto format(const cpp::RRef& obj, std::format_context& ctx) const {
        return std::format_to(ctx.out(), "{}&&", obj.type);
    }
};

#endif /* CPP_TYPE_H */
