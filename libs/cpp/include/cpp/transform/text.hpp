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

    text& operator<<(const std::string& word) {
        data.push_back(word);
        return *this;
    }

    void to_file(std::filesystem::path& path) {
        std::ofstream out_file(path, std::ios::out);
        out_file << *this;
    }

    friend std::ostream& operator<<(std::ostream& out, const text& txt);

    private:
        std::vector<std::string> data;
    };

    std::ostream& operator<<(std::ostream& out, const cpp::formatter::text& txt) {
        for (auto& word: txt.data) {
            out << word;
        }
        return out;
    }

    cpp::formatter::text& operator<<(cpp::formatter::text& text, const general_id_ptr& general_id);

    template<typename ...Types>
    cpp::formatter::text& operator<<(cpp::formatter::text& text, const std::variant<Types...>& general_var) {
        std::visit([&](auto&& var) { text << var; }, general_var);
        return text;
    }

    // (<specifier>, <symbol>,...)
    cpp::formatter::text& operator<<(cpp::formatter::text& text, auto element) {
        using Type = std::decay_t<decltype(element)>;
        using format = Type::format;
        format::check_and_apply([&](auto& el) { text << el; }, element.begin(), element.end());
        return text;
    }

    cpp::formatter::text& operator<<(cpp::formatter::text& text, const language::keyword_t& keyword) {
        std::visit([&](auto&& kw) {
            using Type = std::decay_t<decltype(kw)>;
            text << Type::keyword;
        }, keyword);
        return text;
    }

    cpp::formatter::text& operator<<(cpp::formatter::text& text, const language::symbol_t& symbol) {
        std::visit([&](auto&& symbol) {
            using Type = std::decay_t<decltype(symbol)>;
            text << Type::symbol;
        }, symbol);
        return text;
    }

    cpp::formatter::text& operator<<(cpp::formatter::text& text, const std::shared_ptr<cpp::simple_type_specifier_t>& st) {
        text << st->id();
        return text;
    }

    template<typename Derrived>
    void do_if_derrived(const std::shared_ptr<cpp::specifier_t>& el, auto&& func) {
        if (auto&& derrived_ptr = std::dynamic_pointer_cast<Derrived>(el); derrived_ptr) {
            language::keyword_t keyword = derrived_ptr->keyword;
            func(keyword);
        }
    }

    template<typename ...Derrived>
    void do_for_all_derrived(const std::shared_ptr<cpp::specifier_t>& el, auto&& func) {
        (do_if_derrived<Derrived>(el, func),...);
    }

    cpp::formatter::text& operator<<(cpp::formatter::text& text, const std::shared_ptr<cpp::specifier_t>& el) {
        if (auto&& st_ptr = std::dynamic_pointer_cast<simple_type_specifier_t>(el); st_ptr) {
            text << st_ptr;
        }
        auto print = [&](auto&& keyword) { text << keyword; };
        do_for_all_derrived<cpp::static_specifier_t,
                            cpp::extern_specifier_t,
                            cpp::constexpr_specifier_t,
                            cpp::volatile_qualifier_t,
                            cpp::const_qualifier_t>(el, print);

        return text;
    }

    cpp::formatter::text& operator<<(cpp::formatter::text& text, const unqualified_id_t& unqualified_id) {
        text << unqualified_id.id();
        return text;
    }

    cpp::formatter::text& operator<<(cpp::formatter::text& text, const qualified_id_t& qualified_id) {
        text << qualified_id.prefix() << qualified_id.collons << qualified_id.id();
        return text;
    }

    cpp::formatter::text& operator<<(cpp::formatter::text& text, const general_id_ptr& general_id) {
        if (auto qual_ptr = std::dynamic_pointer_cast<qualified_id_t>(general_id); qual_ptr) {
            text << *qual_ptr;
        } else if (auto unqual_id = std::dynamic_pointer_cast<unqualified_id_t>(general_id); unqual_id) {
            text << *unqual_id;
        }
        return text;
    }

    cpp::formatter::text& operator<<(cpp::formatter::text& text, const direct_initialization_t& copy_init) {
        throw std::runtime_error("Not implemented yet for direct_initialization_t!");
        return text;
    }

    cpp::formatter::text& operator<<(cpp::formatter::text& text, const list_initialization_t& copy_init) {
        throw std::runtime_error("Not implemented yet for list_initialization_t!");
        return text;
    }

    cpp::formatter::text& operator<<(cpp::formatter::text& text, const expression_t& expr) {
        text << expr.expression();
        return text;
    }

    cpp::formatter::text& operator<<(cpp::formatter::text& text, const copy_initialization_t& copy_init) {
        // TODO: Monkey code to refactor
        text << language::symbol_t(copy_init.assignment) << language::symbol_t(language::space_t {}) << copy_init.expression();
        return text;
    }

    cpp::formatter::text& operator<<(cpp::formatter::text& text, const init_declarator_t& init_declarator) {
        text << init_declarator.declarator();
        if (auto initializer = init_declarator.initializer(); initializer.has_value()) {
            return text << language::symbol_t(language::space_t {}) << initializer.value();
        }
        return text;
    }

    cpp::formatter::text& operator<<(cpp::formatter::text& text, const simple_declaration_t& simple_declaration) {
        text << simple_declaration.decl_spec_seq()
             << language::symbol_t(simple_declaration.separator)
             << simple_declaration.init_decl_list()
             << language::symbol_t(simple_declaration.trailer);
        return text;
    }
}
