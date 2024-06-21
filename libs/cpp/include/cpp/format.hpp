#pragma once

#include <variant>
#include <iterator>
#include <type_traits>

#include "cpp/symbols.hpp"
#include "cpp/declaration/specifier.hpp"

namespace language {

    template<typename T, typename Var>
    struct contains_t {};

    template<typename T, typename ...Ts>
    struct contains_t<T, std::variant<Ts...>> : std::bool_constant<(... || std::is_same<T, Ts> {})> {};

    template<typename ...Types>
    class zero_or_more {};

    template<typename T>
    struct check_and_apply_impl_t {
        static void apply(auto&& func, auto& iter, const auto end) {
            if constexpr (contains_t<T, language::keyword_t>::value) {
                language::keyword_t&& keyword = T {};
                func(keyword);
                //auto&& keyword = std::get<language::keyword_t>(*iter);
                //if (std::holds_alternative<T>(keyword)) {
                //    func(keyword);
                //} else {
                //    throw std::runtime_error("Keyword doesn't hold required format!");
                //}
            } else if constexpr (contains_t<T, language::symbol_t>::value) {
                language::symbol_t symbol = T {};
                func(symbol);
                //auto&& symbol = std::get<language::symbol_t>(*iter);
                //if (std::holds_alternative<T>(symbol)) {
                //    func(symbol);
                //} else {
                //    throw std::runtime_error("Symbol doesn't hold required format!");
                //}
            } else if constexpr (std::is_same_v<std::iter_value_t<decltype(iter)>, language::extensible_ptr>) {
                auto ptr = std::dynamic_pointer_cast<T>(*iter);
                func(ptr);
                ++iter;
            } else {
                func(*iter);
                ++iter;
            }
        }
    };

    template<typename ...Types>
    struct check_and_apply_impl_t<zero_or_more<Types...>> {
        static void apply(auto&& func, auto& iter, const auto end) {
            while (iter != end) {
                (check_and_apply_impl_t<Types>::apply(func, iter, end),...);
            }
        }
    };

    template<typename T>
    struct check_and_apply_impl_t<std::vector<T>> {
        static void apply(auto&& func, auto& iter, const auto end) {
            auto& vec = *iter;
            for (auto&v : vec) {
                func(v);
                ++iter;
                // TODO: somehow add borders check
            }
        }
    };

    template<typename T>
    struct check_and_apply_impl_t<std::optional<T>> {
        static void apply(auto&& func, auto& iter, const auto end) {
            if (iter->has_value()) {
                func(iter.value());
                ++iter;
                // TODO: somehow add borders check
            }
        }
    };

    // (<specifier>, <symbol>,...)
    // using decl_spec_seq = format<specifier_t, zero_or_more<language::space_t, specifier_t>>;
    template<typename ...Types>
    class format_t {
    public:
        static void check_and_apply(auto&& func, auto iter, auto end) {
            (check_and_apply_impl_t<Types>::apply(func, iter, end),...);
            // apply_impl<specifier_t>(),
            // apply_impl<zero_or_more<space_t, specifier_t>>()
        }
    };
};
