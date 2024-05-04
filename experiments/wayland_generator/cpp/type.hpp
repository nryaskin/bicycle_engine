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


    class Types {
    private:
    public:
        Types() {}
        void add_type(Type type);
    private:
        std::set<Type> types;
    };
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

#endif /* CPP_TYPE_H */
