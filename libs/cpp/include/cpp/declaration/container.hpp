#ifndef CPP_CONTAINER_H
#define CPP_CONTAINER_H

#include <concepts>
#include <variant>
#include <vector>
#include <type_traits>

namespace cpp {
    template<typename T, typename ...Types>
    concept Containable = (std::same_as<T, Types> || ...);

    template <typename T>
    concept not_monostate = !std::same_as<std::decay_t<T>, std::monostate>;

    template<typename ...Types>
    class Container : public std::vector<std::variant<std::monostate, Types...>> {
    public:

        template<typename T>
        void add(T&& t) requires Containable<typename std::remove_reference<T>::type, Types...> {
            push_back(std::forward<T>(t));
        }

        template<typename ...SomeTypes>
        void add(SomeTypes ...types) {
            this->insert(this->end(), {types...});
        }
    };
}

#endif /* CPP_CONTAINER_H */
