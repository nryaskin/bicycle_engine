#ifndef WAYLAND_GENERATOR_CPP_BUILDER_H
#define WAYLAND_GENERATOR_CPP_BUILDER_H

#include "protocol.hpp"
#include "cpp/incl.hpp"
#include "cpp/file.hpp"
#include "cpp/type.hpp"

namespace wayland::generator {
    class Builder {
    public:
        // As variable mapper of wayland types to wire_type.h types I think.
        Builder(cpp::Includes common_includes);

        std::tuple<cpp::Header, cpp::Source> build(const wayland::generator::WLInterface&);

    private:
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
        cpp::Type                socket_type = cpp::Type("WLSocket");
        cpp::Type                void_type   = cpp::Type("void");
    };
}

#endif /* WAYLAND_GENERATOR_CPP_BUILDER_H */
