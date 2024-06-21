#include "cpp/declaration/specifier.hpp"
#include "cpp/declaration/declarator.hpp"
#include "cpp/declaration/initializer.hpp"
#include "cpp/declaration/init_declarator_list.hpp"
#include "cpp/declaration/simple_declarator.hpp"
#include "cpp/declaration/statement.hpp"
#include "cpp/declaration/function.hpp"
#include "cpp/declaration/class.hpp"

#include "cpp/transform/text.hpp"

#include <iostream>

int main() {
    // constexpr std::string
    cpp::decl_specifier_seq_t constexpr_string(cpp::static_specifier, cpp::constexpr_specifier, cpp::simple_type_specifier_t("char"));

    // && car
    cpp::declarator_t c_value = cpp::rvalue_reference_declarator_t(cpp::unqualified_id_t::make("car"));

    // = "some"
    cpp::initializer_t copy_init = cpp::copy_initialization_t(cpp::expression_t("\"some\""));

    // && c = "some"
    cpp::init_declarator_t init_declarator(c_value, copy_init);
    cpp::init_declarator_list_t init_declarator_list(init_declarator);

    // constexpr std::string &&c = "some";
    cpp::simple_declaration_t c_string_init_sd(constexpr_string, init_declarator_list);

    cpp::label_statement_t label_statement(cpp::label_statement_t::label_t::CASE, "THIS");

    cpp::decl_specifier_seq_t _int(cpp::simple_type_specifier_t("int"));
    cpp::function_declaration_t add_two_decl(cpp::unqualified_id_t("add_two"),
                                             cpp::parameter_list_t());
    cpp::compound_statement_t function_body;
    function_body.push_back(std::make_shared<cpp::expression_statement_t>(cpp::expression_t("1 + 1")));
    cpp::function_t add_two(_int, add_two_decl, function_body);

    cpp::clas animal("Animal");

    cpp::formatter::text txt;
    // txt << constexpr_string;
    // txt << c_value;
    // txt << copy_init;
    // txt << init_declarator_list;
    // txt << c_string_init_sd;
    // txt << label_statement;
    //txt << add_two;
    txt << animal;
    std::cout << txt;

    return 0;
}
