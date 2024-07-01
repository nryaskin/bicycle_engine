#pragma once

#include <ranges>
#include <vector>
#include <string>
#include <stdexcept>
#include <ranges>
#include <filesystem>
#include <fstream>

#include "cpp/declaration/specifier.hpp"
#include "cpp/declaration/declarator.hpp"
#include "cpp/declaration/initializer.hpp"
#include "cpp/declaration/init_declarator_list.hpp"
#include "cpp/declaration/simple_declarator.hpp"
#include "cpp/declaration/statement.hpp"

namespace cpp::formatter {
    class text {
    public:
        enum class ident_t {
            add,
            sub
        };

        text& operator<<(const std::string& word) {
            if (!dirty) {
                data.push_back(std::string(identation, ' '));
                dirty = true;
            }
            data.push_back(word);
            return *this;
        }

        text& operator<<(const ident_t& id) {
            switch(id) {
                case ident_t::add: 
                    identation += 4;
                    break;
                case ident_t::sub:
                    identation -= 4;
                    break;
            }
            return *this;
        }

        void newline() {
            dirty = false;
        }

        void to_file(std::filesystem::path& path) {
            std::ofstream out_file(path, std::ios::out);
            out_file << *this;
        }

        friend std::ostream& operator<<(std::ostream& out, const text& txt);
    private:
        std::vector<std::string> data;
        int                      identation = 0;
        bool                     dirty = false;
    };

    inline std::ostream& operator<<(std::ostream& out, const cpp::formatter::text& txt) {
        for (auto& word: txt.data) {
            out << word;
        }
        return out;
    }

    cpp::formatter::text& operator<<(cpp::formatter::text& text, const general_id_ptr& general_id);

    inline cpp::formatter::text& operator<<(cpp::formatter::text& text, const auto& var) {
        auto print = [&](auto&& o) { return text << o; };
        var.sequential_all(print);
        return text;
    }

    template<typename ...Types>
    inline cpp::formatter::text& operator<<(cpp::formatter::text& text, const std::variant<Types...>& general_var) {
        std::visit([&](auto&& var) { text << var; }, general_var);
        return text;
    }

//   // (<specifier>, <symbol>,...)
//   cpp::formatter::text& operator<<(cpp::formatter::text& text, auto element) {
//       using Type = std::decay_t<decltype(element)>;
//       using format = Type::format;
//       format::check_and_apply([&](auto& el) { text << el; }, element.begin(), element.end());
//       return text;
//   }

    inline cpp::formatter::text& operator<<(cpp::formatter::text& text, const language::keyword_t& keyword) {
        std::visit([&](auto&& kw) {
            using Type = std::decay_t<decltype(kw)>;
            text << Type::keyword;
        }, keyword);
        return text;
    }

    inline cpp::formatter::text& operator<<(cpp::formatter::text& text, const language::symbol_t& symbol) {
        std::visit([&](auto&& symbol) {
            using Type = std::decay_t<decltype(symbol)>;
            if constexpr (std::same_as<Type, language::open_curly_brace_t>) {
                text << cpp::formatter::text::ident_t::add;
            }
            if constexpr (std::same_as<Type, language::close_curly_brace_t>) {
                text << cpp::formatter::text::ident_t::sub;
            }
            text << Type::symbol;

            if constexpr (std::same_as<Type, language::newline_t>) {
                text.newline();
            }

        }, symbol);
        return text;
    }

    inline cpp::formatter::text& operator<<(cpp::formatter::text& text, const expression_t& expr) {
        text << expr.expression();
        return text;
    }

    inline cpp::formatter::text& operator<<(cpp::formatter::text& text, const cpp::simple_type_specifier_t& st) {
        text << st.id();
        return text;
    }

    inline cpp::formatter::text& operator<<(cpp::formatter::text& text, const unqualified_id_t& unqualified_id) {
        text << unqualified_id.id();
        return text;
    }

    inline cpp::formatter::text& operator<<(cpp::formatter::text& text, const general_id_ptr& general_id) {
        if (auto qual_ptr = std::dynamic_pointer_cast<qualified_id_t>(general_id); qual_ptr) {
            text << *qual_ptr;
        } else if (auto unqual_id = std::dynamic_pointer_cast<unqualified_id_t>(general_id); unqual_id) {
            text << *unqual_id;
        }
        return text;
    }

    template<typename Derrived>
    inline void do_if_derrived(const auto& el, auto&& func) {
        if (auto&& derrived_ptr = std::dynamic_pointer_cast<Derrived>(el); derrived_ptr) {
            func(*derrived_ptr);
        }
    }

    template<typename ...Derrived>
    inline void do_for_all_derrived(const auto& el, auto&& func) {
        (do_if_derrived<Derrived>(el, func),...);
    }

    inline cpp::formatter::text& operator<<(cpp::formatter::text& text, const cpp::statement_ptr& stmt) {
        auto print = [&](auto&& keyword) { text << keyword; };
        do_for_all_derrived<cpp::label_statement_t,
                            cpp::expression_statement_t,
                            cpp::compound_statement_t,
                            cpp::switch_statement_t, cpp::if_statement_t,
                            cpp::for_statement_t, while_statement_t>(stmt, print);

        return text;
    }
}
