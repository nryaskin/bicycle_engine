#ifndef CPP_CONTAINER_H
#define CPP_CONTAINER_H

#include <concepts>
#include <variant>
#include <vector>
#include <type_traits>

namespace cpp {
    template<typename T, typename ...Types>
    concept Containable = (std::same_as<T, Types> || ...);

    template<typename ...Types>
    class Container {
    public:
        using var_t = std::variant<std::monostate, Types...>;

        template<typename T>
        void add(T&& t) requires Containable<typename std::remove_reference<T>::type, Types...> {
            container.push_back(std::forward<T>(t));
        }
        
    protected:
        std::vector<var_t> container;
    };
}

#endif /* CPP_CONTAINER_H */
