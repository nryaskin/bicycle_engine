#ifndef CPP_ACCESS_MOD_H
#define CPP_ACCESS_MOD_H
#include <string>

namespace cpp {
    enum class access_modifier_t {
        PUBLIC,
        PROTECTED,
        PRIVATE
    };

    class AccessModifier {
    public:
        AccessModifier(access_modifier_t mod) : mod(mod) {}
        std::string to_string() const {
            switch(mod) {
            case access_modifier_t::PRIVATE:
                return "private:";
            case access_modifier_t::PUBLIC:
                return "public:";
            case access_modifier_t::PROTECTED:
                return "protected:";
            }
            return "";
        }
    private:
        access_modifier_t mod;
    };
}

#endif /* CPP_ACCESS_MOD_H */
