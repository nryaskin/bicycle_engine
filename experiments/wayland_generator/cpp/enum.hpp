#ifndef CPP_ENUM_H
#define CPP_ENUM_H
#include <string>
#include <vector>
#include <sstream>

#include "type.hpp"

namespace cpp {
    // TODO: I am going to implement only enum class which inherits from some value and don't do anything else.
    class Enum {
    public:
        class Entity {
        public:
            Entity(const std::string& name, const std::string& value) : name(name), value(value) {}

            friend class Enum;
            friend Document& operator<<(Document&, const Entity&);
        private:
            std::string name;
            std::string value;
        };

        Enum(const std::string& name, type_t base) : name(name), base(base) {}

        void add(Entity e) {
            entities.push_back(e);
        }

        friend Document& operator<<(Document&, const Enum&);

    private:
        std::string name;
        type_t base;
        std::vector<Entity> entities;
    };
}

#endif /* CPP_ENUM_H */
