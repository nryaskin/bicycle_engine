#pragma once

#include <memory>

#include "cpp/declaration/initializer.hpp"
#include "cpp/declaration/simple_declarator.hpp"
#include "cpp/symbols.hpp"
#include "cpp/keywords.hpp"
#include "cpp/format.hpp"

namespace cpp {

    class statement_t {
    public:
        virtual ~statement_t() {}
    };

    using statement_ptr = std::shared_ptr<statement_t>;
    using statement_vec = std::vector<statement_ptr>;

    // C++ includes following types of statements:
    // 1) labeled statements;  <- needed for case
    // 2) expression statements; <- just ok to have
    // 3) compound statements; <- { statement... } need for functions
    // 4) selection statements; <- if, switch statements ok to have
    // 5) iteration statements; <- for, while ok to have
    // 6) jump statements; <- break, return, continue (Will need `return` at least.)
    // 7) declaration statements; <- I've already implemented declarations
    // 8) try blocks;

    // identifier: (1)  <- target for goto
    // case `constexpr`: (2) <- case label in switch statement
    // default:          (3) <- default label in switch statement
    class label_statement_t : public statement_t {
    public:
       enum class label_t {
            GOTO,
            CASE,
            DEFAULT
        };

        label_statement_t() : label_type_(label_t::DEFAULT) {}
        explicit label_statement_t(label_t label_type = label_t::DEFAULT, const std::string& label = "")
        : label_type_(label_type),
          label_(label) {}
        auto label_type() const { return label_type_; }
        const auto& label() const { return label_; }

        void sequential_all(auto&& action) const {
            switch (label_type()) {
                case label_t::GOTO: {
                    action(language::goto_keyword);
                }
                break;
                case label_t::CASE: {
                    action(language::case_keyword);
                    action(language::space);
                    action(label());
                }
                break;
                case label_t::DEFAULT: {
                    action(language::default_keyword);
                }
                break;
            }
            action(language::collon);
        }

    private:
        label_t     label_type_;
        std::string label_;
    };

    // expression ;
    class expression_statement_t : public statement_t {
    public:
        static constexpr auto trailer = language::semi_collon_t {};
        explicit expression_statement_t(expression_t expression) : expression_(expression) {}
         
        auto& expression() { return expression_; }
        const auto& expression() const { return expression_; }
        void sequential_all(auto&& action) const {
            action(expression());
            action(language::semi_collon);
        }
    private:
        expression_t expression_;
    };

    // { statement... }
    class compound_statement_t : public statement_t, public statement_vec {
    public:
        void sequential_all(auto&& action) const {
            const statement_vec& vec = *this;
            action(language::open_curly_brace);
            action(language::newline);
            for (auto& statement : vec) {
                action(statement);
                action(language::newline);
            }
            action(language::close_curly_brace);
        }
    };

    using init_statement_t = std::variant<expression_statement_t, simple_declaration_t>;
    using condition_t = expression_t;

    // if (init-statement(optional) condition) statement                (1)
    // if (init-statement(optional) condition) statement else statement (2)
    // switch (init-statement(optional) condition) statement            (3)
    //
    // init-statement - expression statement, simple declaration
    // condition - expression, declaration
    // statement - any statement(typically a compound).case: and default: are permitted
    class if_statement_t : public statement_t, statement_vec {
    public:
        explicit if_statement_t(condition_t condition, statement_ptr statement)
            : condition_(condition),
              statement_(statement) {}

        explicit if_statement_t(init_statement_t init_statement, condition_t condition, statement_ptr statement)
            : init_statement_(init_statement),
              condition_(condition),
              statement_(statement) {}

        void sequential_all(auto&& action) const {
            action(language::if_keyword);
            action(language::open_brace);
            if (init_statement_) {
                action(init_statement_.value());
            }
            action(condition_);
            action(language::close_brace);
            action(statement_);
        }
    private:
        std::optional<init_statement_t>             init_statement_;
        condition_t                                 condition_;
        statement_ptr                               statement_;
    };

    class switch_statement_t : public statement_t {
    public:
        explicit switch_statement_t(condition_t condition, statement_ptr statement)
            : condition_(condition),
              statement_(statement) {}

        explicit switch_statement_t(init_statement_t init_statement, condition_t condition, statement_ptr statement)
            : init_statement_(init_statement),
              condition_(condition),
              statement_(statement) {}

        void sequential_all(auto&& action) const {
            action(language::switch_keyword);
            action(language::open_brace);
            if (init_statement_) {
                action(init_statement_.value());
            }
            action(condition_);
            action(language::close_brace);
            action(statement_);
        }
    private:
        std::optional<init_statement_t> init_statement_;
        condition_t                     condition_;
        statement_ptr                   statement_;
    };

    using conditino_statement_t = std::variant<if_statement_t, switch_statement_t>;

    class while_statement_t : public statement_t {
    public:
        explicit while_statement_t(condition_t condition, statement_ptr statement)
            : condition_(condition),
              statement_(statement) {}
        const auto& condition() const { return condition_; }
        const auto& statement() const { return statement_; }

        void sequential_all(auto&& action) const {
            action(language::while_keyword);
            action(language::open_brace);
            action(condition_);
            action(language::close_brace);
            action(statement_);
        }

    private:
        condition_t   condition_;
        statement_ptr statement_;
    };


    class for_statement_t : public statement_t {
    public:
        explicit for_statement_t(init_statement_t init_statement, statement_ptr statement)
            : init_statement_(init_statement),
              statement_(statement) {}

        explicit for_statement_t(init_statement_t init_statement, condition_t condition, expression_t expression, statement_ptr statement)
            : init_statement_(init_statement),
              condition_(condition),
              expression_(condition),
              statement_(statement) {}

        void sequential_all(auto&& action) const {
            action(language::for_keyword);
            action(init_statement_);
            action(language::semi_collon);
            if (condition_) {
                action(condition_.value());
            }
            action(language::semi_collon);
            if (expression_) {
                action(expression_.value());
            }
            action(statement_);
        }

    private:
        init_statement_t            init_statement_;
        std::optional<condition_t>  condition_;
        std::optional<expression_t> expression_;
        statement_ptr               statement_;
    };

    // while(condition) statement                                               (1)
    // for (init-statement condition(optional); expression(optional)) statement (2)
    using iteration_statement_t = std::variant<while_statement_t, for_statement_t>;

    // break;                           (1)
    // continue;                        (2)
    // return expression(optional) ;    (3)
    // return braced-init-list;         (4)
    class jump_statement_t : public statement_t {
    public:
        enum class jump_t {
            BREAK,
            CONTINUE,
            RETURN
        };
        explicit jump_statement_t(jump_t jump_type) : jump_type_(jump_type) {}
        static auto return_statement(expression_t expression) {
            return jump_statement_t(jump_t::RETURN, expression);
        }

        void sequential_all(auto&& action) const {
            switch(jump_type_) {
                case jump_t::BREAK: {
                    action(language::break_keyword);
                }
                break;
                case jump_t::CONTINUE: {
                    action(language::continue_keyword);
                }
                break;
                case jump_t::RETURN: {
                    action(language::return_keyword);
                    if (expression_) {
                        action(expression_.value());
                    }
                }
                break;
            }
            action(language::semi_collon);
        }

    private:
        jump_statement_t(jump_t jump_type, expression_t expression)
            : jump_type_(jump_type),
              expression_(expression) {}
        jump_t jump_type_;
        std::optional<expression_t> expression_;
    };

    // declaration
    class declaration_statement_t {

    };
};
