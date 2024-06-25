#pragma once

#include "protocol.hpp"
#include "cpp/declaration/includes.hpp"
#include "cpp/declaration/function.hpp"
#include "cpp/declaration/class.hpp"
#include "cpp/transform/text.hpp"

namespace wayland::generator {
    class Builder {
    public:
        // As variable mapper of wayland types to wire_type.h types I think.
        Builder(std::vector<cpp::include_t> common_includes) : common_includes(common_includes) {}

        cpp::formatter::text build(const wayland::generator::WLInterface&);

    private:
        std::vector<cpp::include_t> common_includes;
        cpp::simple_type_specifier_t wire_to_type(wire_type type);
        cpp::simple_type_specifier_t socket_type = cpp::simple_type_specifier_t("waylandcpp::wire::WLSocket");
        cpp::simple_type_specifier_t void_type   = cpp::simple_type_specifier_t("void");
        cpp::simple_type_specifier_t string_type = cpp::simple_type_specifier_t("std::string");
        std::vector<cpp::function_t> gen_requests(const std::vector<WLRequest>&);
        std::vector<cpp::function_t> gen_events(const std::vector<WLEvent>&);
        std::vector<cpp::simple_declaration_t> gen_variables(const WLInterface& interface);
        cpp::parameter_list_t gen_parameters(const std::vector<WLArgument>&);
#if 0
        std::vector<cpp::Enum>   gen_enums(const std::vector<WLEnum>&);
        cpp::MethodBody          gen_request_body(const cpp::Method&);
        cpp::MethodBody          gen_ctr_body(const cpp::Class::Ctr&);

        cpp::Definition          gen_definition(const cpp::Class&, const cpp::Method&);
        cpp::Definition          gen_definition(const cpp::Class&, const cpp::Class::Ctr&);

        std::vector<cpp::SimpleDeclaration> gen_variables(const WLInterface&);

        // TODO: I can track what I am including and where definitions are comming from but I don't want to do it now.
        // Includes that will be used in each file.
        cpp::Includes            common_includes;
        std::vector<cpp::type_t> common_types;
        cpp::Type                socket_type = cpp::Type("waylandcpp::wire::WLSocket");
        cpp::Type                void_type   = cpp::Type("void");
#endif
    };
}

