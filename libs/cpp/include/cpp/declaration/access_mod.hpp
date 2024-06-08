#ifndef CPP_ACCESS_MOD_H
#define CPP_ACCESS_MOD_H
#include <string>

namespace cpp {
    class Document;
    enum class access_modifier_t {
        PUBLIC,
        PROTECTED,
        PRIVATE
    };

    class AccessModifier {
    public:
        AccessModifier(access_modifier_t mod) : mod(mod) {}

        friend Document& operator<<(Document& doc, const AccessModifier& am);
    private:
        access_modifier_t mod;
    };
}

#endif /* CPP_ACCESS_MOD_H */
