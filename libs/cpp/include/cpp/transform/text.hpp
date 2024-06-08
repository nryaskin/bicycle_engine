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

    cpp::formatter::text& operator<<(cpp::formatter::text&, const cpp::initializer_clause_t&);
    cpp::formatter::text& operator<<(cpp::formatter::text& txt, const cpp::initializer_list_t& li);

    auto operator<<(cpp::formatter::text& txt, const auto& value) {
        txt << std::decay_t<decltype(value)>::keyword;
        return txt;
    }

    auto operator<<(cpp::formatter::text& txt, const cpp::simple_type_specifier_t& simple_type_specifier) {
        txt << simple_type_specifier.id();
        return txt;
    }

    auto operator<<(cpp::formatter::text& txt, const cpp::decl_specifier_seq_t& decl_spec_seq) {
        auto it = decl_spec_seq.begin();
        auto print = [&] (auto&& obj) { txt << obj; };
        std::visit(print, *it);
        while (++it != decl_spec_seq.end()) {
            txt << cpp::decl_specifier_seq_t::separator;
            std::visit(print, *it);
        }
        return txt;
    }

    auto operator<<(cpp::formatter::text& txt, const cpp::unqualified_id_t& unq_id) {
        txt << unq_id.id();
        return txt;
    }

    auto operator<<(cpp::formatter::text& txt, const cpp::declarator_t& declarator) {
        std::visit([&] (auto && obj) { txt << obj; }, declarator);
        return txt;
    }


    auto operator<<(cpp::formatter::text& txt, const cpp::qualified_id_t& q_id) {
        for (auto& unq_id : q_id.prefix()) {
            txt << unq_id << cpp::qualified_id_t::collons;
        }
        txt << q_id.id();
        return txt;
    }

    auto operator<<(cpp::formatter::text& txt, const cpp::lvalue_reference_declarator_t& lvalue_ref) {
        txt << cpp::lvalue_reference_declarator_t::ref;
        std::visit([&](auto &&id) { txt << id; }, lvalue_ref.declarator());
        return txt;
    }

    auto operator<<(cpp::formatter::text& txt, const cpp::rvalue_reference_declarator_t& rvalue_ref) {
        txt << cpp::rvalue_reference_declarator_t::rref;
        std::visit([&](auto &&id) { txt << id; }, rvalue_ref.declarator());
        return txt;
    }

    auto operator<<(cpp::formatter::text& txt, const cpp::expression_t& expression) {
        txt << expression.expression();
        return txt;
    }

    auto operator<<(cpp::formatter::text& txt, const cpp::expression_list_t& e_list) {
        auto&& expressions = e_list.expressions();
        auto it = expressions.begin();

        txt << *it;
        while (++it < expressions.end()) {
            txt << cpp::expression_list_t::separator;
            txt << *it;
        }
        return txt;
    }

    auto operator<<(cpp::formatter::text& txt, const cpp::list_initialization_t& li) {
        if (li.assign) {
            txt << cpp::symbols::space << cpp::list_initialization_t::assignment << cpp::symbols::space;
        }
        auto && init_list = li.init_list();
        using init_list_type = std::decay_t<decltype(init_list)>;
        txt << init_list_type::open << cpp::symbols::space;

        if (init_list.init_list().has_value()) {
            txt << init_list.init_list().value() << cpp::symbols::space;
        }

        txt << init_list_type::close;
        return txt;
    }


    cpp::formatter::text& operator<<(cpp::formatter::text& txt, const cpp::initializer_list_t& li) {
        auto&& l = li.list();
        auto it = l.begin();

        txt << *it;
        while (++it != l.end()) {
            txt << cpp::initializer_list_t::separator << *it;
        }

        return txt;
    }

    cpp::formatter::text& operator<<(cpp::formatter::text& txt, const cpp::initializer_clause_t& init_clause) {
        std::visit([&](auto&& clause) { txt << clause; }, init_clause);
        return txt;
    }

    auto operator<<(cpp::formatter::text& txt, const cpp::copy_initialization_t& ci) {
        txt << cpp::symbols::space << cpp::copy_initialization_t::assignment << cpp::symbols::space << ci.expression();
        return txt;
    }

    auto operator<<(cpp::formatter::text& txt, const cpp::direct_initialization_t& di) {
        txt << cpp::direct_initialization_t::open << cpp::symbols::space;
        std::visit([&](auto&& l) { txt << l; }, di.list());
        txt << cpp::symbols::space << cpp::direct_initialization_t::close;
        return txt;
    }

    auto operator<<(cpp::formatter::text& txt, const cpp::initializer_t& initializer) {
        std::visit([&](auto&& initialization) { txt << initialization; }, initializer);
        return txt;
    }

    auto operator<<(cpp::formatter::text& txt, const cpp::init_declarator_t& init_decl) {
        txt << init_decl.declarator();
        const auto& initializer = init_decl.initializer();
        if (initializer.has_value()) {
            txt << cpp::init_declarator_t::separator;
            txt << initializer.value();
        }
        return txt;
    }

    auto operator<<(cpp::formatter::text& txt, const cpp::init_declarator_list_t& init_decl_list) {
        auto& init_declarators = init_decl_list.init_declarators();
        auto it = init_declarators.begin();
        txt << *it;
        while(++it != init_declarators.end()) {
            txt << cpp::init_declarator_list_t::separator;
            txt << *it;
        }
        return txt;
    }

    auto operator<<(cpp::formatter::text& txt, const cpp::simple_declaration_t& init_decl_list) {
        txt << init_decl_list.decl_spec_seq();
        txt << cpp::simple_declaration_t::separator;
        txt << init_decl_list.init_decl_list();
        txt << std::decay_t<decltype(init_decl_list)>::close;
        return txt;
    }
}
