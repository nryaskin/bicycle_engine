#pragma once

#include "protocol.hpp"
#include "cpp/declaration/includes.hpp"
#include "cpp/declaration/function.hpp"
#include "cpp/declaration/class.hpp"
#include "cpp/transform/text.hpp"
#include "cpp/declaration/enum.hpp"

namespace wayland::generator {
    class Builder {
    public:
        // As variable mapper of wayland types to wire_type.h types I think.
        Builder(std::vector<cpp::include_t> common_includes) : common_includes(common_includes) {}

        cpp::formatter::text build(const wayland::generator::WLInterface&);

    private:
        std::vector<cpp::include_t> common_includes;
        cpp::specifier_t wire_to_type(wire_type type);
        // I guess it can be done with qualified_id_t but I want it to work and touch wayland
        cpp::simple_type_specifier_t socket_type = cpp::simple_type_specifier_t("wire::WLSocket");
        cpp::simple_type_specifier_t void_type   = cpp::simple_type_specifier_t("void");
        cpp::simple_type_specifier_t string_type = cpp::simple_type_specifier_t("std::string");
        cpp::simple_type_specifier_t wire_object_builder_type = cpp::simple_type_specifier_t("wire::WireObjectBuilder");
        std::vector<cpp::function_t> gen_requests(const std::vector<WLRequest>&);
        std::vector<cpp::function_t> gen_events(const std::vector<WLEvent>&);
        std::vector<cpp::simple_declaration_t> gen_variables(const WLInterface& interface);
        cpp::function_t gen_dispatcher(const WLInterface& interface);
        cpp::parameter_list_t gen_parameters(const std::vector<WLArgument>&);
        cpp::compound_statement_t gen_request_body(const cpp::function_declaration_t& req);
        std::vector<cpp::enum_specifier_t> gen_enums(const std::vector<WLEnum>& enums);
    };
}

