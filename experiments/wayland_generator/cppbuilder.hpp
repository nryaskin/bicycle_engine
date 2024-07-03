#pragma once

#include "protocol.hpp"
#include "cpp/declaration/includes.hpp"
#include "cpp/transform/text.hpp"

namespace wayland::generator {
    class Builder {
    public:
        // As variable mapper of wayland types to wire_type.h types I think.
        Builder(std::vector<cpp::include_t> common_includes) : common_includes(common_includes) {}

        cpp::formatter::text build(const wayland::generator::WLInterface&);

    private:
        std::vector<cpp::include_t> common_includes;
#if 0
        std::vector<cpp::Enum>   gen_enums(const std::vector<WLEnum>&);
        std::vector<cpp::Method> gen_requests(const std::vector<WLRequest>&);
        std::vector<cpp::Method> gen_events(const std::vector<WLEvent>&);
        cpp::MethodBody          gen_request_body(const cpp::Method&);
        cpp::MethodBody          gen_ctr_body(const cpp::Class::Ctr&);

        cpp::Definition          gen_definition(const cpp::Class&, const cpp::Method&);
        cpp::Definition          gen_definition(const cpp::Class&, const cpp::Class::Ctr&);
        std::vector<cpp::Parameter> gen_parameters(const std::vector<WLArgument>&);

        std::vector<cpp::SimpleDeclaration> gen_variables(const WLInterface&);

        cpp::Type wire_to_type(wire_type type);
        // TODO: I can track what I am including and where definitions are comming from but I don't want to do it now.
        // Includes that will be used in each file.
        cpp::Includes            common_includes;
        std::vector<cpp::type_t> common_types;
        cpp::Type                socket_type = cpp::Type("waylandcpp::wire::WLSocket");
        cpp::Type                void_type   = cpp::Type("void");
#endif
    };
}
