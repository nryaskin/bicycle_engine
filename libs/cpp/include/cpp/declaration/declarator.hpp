#pragma once

#include <vector>
#include <variant>
#include <string>

#include "cpp/symbols.hpp"
#include "cpp/specification.hpp"

namespace cpp {
    class general_id_t {
    public:
        virtual ~general_id_t() {}
    };
    using general_id_ptr = std::shared_ptr<general_id_t>;

    // just name that is declared
    class unqualified_id_t : public general_id_t {
    public:
        unqualified_id_t() = default;
        unqualified_id_t(const std::string& id) : id_(id) {}

        unqualified_id_t(const unqualified_id_t& unqid) {
            *this = unqid;
        }

        unqualified_id_t& operator=(const unqualified_id_t& unquid) {
            id_ = unquid.id_;
            return *this;
        }

        const auto& id() const { return id_; }
    private:
        std::string id_;
    };
    using unqid_t = unqualified_id_t;


    // Declaration that uses qualified identifier
    class qualified_id_t : public general_id_t {
    public:
        qualified_id_t(auto prefix, unqid_t unqid) : prefix_(std::make_shared<decltype(prefix)>(prefix)), id_(unqid) {}

        const auto& id() const { return id_; }
        const auto& prefix() const { return prefix_; }

        void sequential_all(auto&& action) const {
            action(prefix_);
            action(language::collon);
            action(language::collon);
            action(id_);
        }
    private:
        general_id_ptr prefix_;
        unqualified_id_t id_;
    };
    using qid_t = qualified_id_t;


    // & declarator (Even if it is written on cppreference that it is any declarator it cannot be itself which means only two other options;
    class lvalue_reference_declarator_t {
    public:
        lvalue_reference_declarator_t(auto id) {
            id_ = std::make_shared<decltype(id)>(id);
        }

        void sequential_all(auto&& action) const {
            action(language::ampersand);
            action(language::space);
            action(id_);
        }
    private:
        general_id_ptr id_;
    };
    using lval_t = lvalue_reference_declarator_t;

    // && declarator 
    class rvalue_reference_declarator_t {
    public:
        rvalue_reference_declarator_t(auto id) {
            id_ = std::make_shared<decltype(id)>(id);
        }

        void sequential_all(auto&& action) const {
            action(language::ampersand);
            action(language::ampersand);
            action(language::space);
            action(id_);
        }
    private:
        general_id_ptr id_;
    };
    using rval_t = rvalue_reference_declarator_t;

    using declarator_t = std::variant<unqid_t, qid_t, lval_t, rval_t>;
}

