#include "cpp/declaration/specifier.hpp"
#include "cpp/declaration/declarator.hpp"
#include "cpp/declaration/initializer.hpp"
#include "cpp/declaration/init_declarator_list.hpp"
#include "cpp/declaration/simple_declarator.hpp"

#include "cpp/transform/text.hpp"

#include <iostream>

int main() {
    // constexpr std::string
    cpp::decl_specifier_seq_t constexpr_string(std::make_shared<cpp::static_specifier_t>(), std::make_shared<cpp::constexpr_specifier_t>(), std::make_shared<cpp::simple_type_specifier_t>("std::string"));

    // && c
    cpp::declarator_t c_value = cpp::rvalue_reference_declarator_t(cpp::unqualified_id_t::make("c"));

    // = "some"
    cpp::initializer_t copy_init = cpp::copy_initialization_t(cpp::expression_t("\"some\""));

    // && c = "some"
    cpp::init_declarator_t init_declarator(c_value, copy_init);
    cpp::init_declarator_list_t init_declarator_list(init_declarator);

    // constexpr std::string &&c = "some";
    cpp::simple_declaration_t c_string_init_sd(constexpr_string, init_declarator_list);

    cpp::formatter::text txt;
    // txt << constexpr_string;
    // txt << c_value;
    // txt << copy_init;
    // txt << init_declarator_list;
    txt << c_string_init_sd;
    std::cout << txt;

    return 0;
}
