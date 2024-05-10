#include "cppbuilder.hpp"

namespace wayland::generator {

    std::string op_code_name(const std::string& orig) {
        return std::format("{}_op", orig);
    }

    Builder::Builder(cpp::Includes common_includes) : common_includes(common_includes) {
    }

    cpp::Type Builder::wire_to_type(wire_type type) {
        switch (type) {
            case wire_type::FD:
                static cpp::Type wire_fd_type("wire_fd_t");
                return wire_fd_type;
                break;
            case wire_type::FIXED:
                static cpp::Type wire_fixed_type("wire_fixed_t");
                return wire_fixed_type;
                break;
            case wire_type::INT:
                static cpp::Type wire_int_type("wire_int_t");
                return wire_int_type;
                break;
            case wire_type::UINT:
                static cpp::Type wire_uint_type("wire_uint_t");
                return wire_uint_type;
                break;
            case wire_type::NEW_ID:
                static cpp::Type wire_new_id_type("wire_new_id_t");
                return wire_new_id_type;
                break;
            case wire_type::OBJECT:
                static cpp::Type wire_object_type("wire_object_t");
                return wire_new_id_type;
                break;
            case wire_type::ARRAY:
                static cpp::Type wire_array_type("wire_array_t");
                return wire_new_id_type;
                break;
            case wire_type::STRING:
                static cpp::Type wire_string_type("wire_string_t");
                return wire_new_id_type;
                break;
        }
        return void_type;
    }

    std::tuple<cpp::Header, cpp::Source> Builder::build(const wayland::generator::WLInterface& interface) {
        cpp::Header header(interface.name);
        cpp::Source source(interface.name);

        for (auto& include : common_includes.items()) {
            std::visit([&header](auto &&object) {
                using T = std::decay_t<decltype(object)>;
                if constexpr (!std::same_as<T, std::monostate>) {
                    header.add_include(object);
                }
            }, include);
        }

        cpp::Namespace ns("wayland::interface");

        cpp::Class cl(interface.name);

        cl.add(cpp::AccessModifier(cpp::access_modifier_t::PUBLIC));

        cpp::Class::Ctr ctr;
        ctr.add(cpp::Parameter(cpp::Ref(socket_type), "s_"));
        ctr.add(cpp::Parameter(cpp::RRef(cpp::Type("std::string")), "name", " = \"\""));

        cl.add_ctr(ctr);

        auto enums = gen_enums(interface.enums);
        for (auto& e : enums) {
            cl.add(e);
        }

        auto requests = gen_requests(interface.requests);
        for (auto& request : requests) {
            cl.add(request);
            source.add(gen_definition(cl, request));
        }

        auto events = gen_events(interface.events);
        for (auto& event : events) {
            cl.add(event);
            source.add(gen_definition(cl, event));
        }

        cl.add(cpp::AccessModifier(cpp::access_modifier_t::PRIVATE));
        auto variables = gen_variables(interface);
        for (auto& var : variables) {
            cl.add(var);
        }

        ns.add(cl);
        header.add(ns);

        source.set_namespace(ns);

        return { header, source};
    }

    cpp::MethodBody Builder::gen_request_body(const cpp::Method& req) {
         const std::string object_builder_def = std::format("WireObjectBuilder builder(id, {});", op_code_name(req.name()));
         static const std::string write = "s_.write(builder.data(), builder.size());";
         cpp::MethodBody mb;

         mb.add(object_builder_def);

         std::stringstream ss;
         ss << "builder.add(";
         bool comma = false;
         for (const auto& p : req.get_params()) {
             ss << (comma? ", " : "") << p.name();
             comma = true;
         }
         ss << ");";
         mb.add(ss.str());

         mb.add(write);

         return mb;
    }

    cpp::Definition Builder::gen_definition(const cpp::Class& cs,
                                            const cpp::Method& m) {
        return cpp::Definition(cs, m, gen_request_body(m));
    }

    std::vector<cpp::Parameter> Builder::gen_parameters(const std::vector<WLArgument>& args) {
        std::vector<cpp::Parameter> params;

        for (const auto& arg : args) {
            // NOTE: I am doing basic implementation here so I am not going to user enums in case there were enums specified in xml, but I am going to do it latter to have links between types in events and enums.
            cpp::Parameter param(cpp::Ref(wire_to_type(arg.type)), arg.name);
            params.push_back(param);
        }

        return params;
    }

    std::vector<cpp::Method> Builder::gen_requests(const std::vector<WLRequest>& requests) {
        std::vector<cpp::Method> methods;

        for (const auto& req : requests) {
            cpp::Method m(req.name, void_type);
            m.append_parameters(gen_parameters(req.arguments));
            methods.push_back(m);
        }

        return methods;
    }

    std::vector<cpp::Enum> Builder::gen_enums(const std::vector<WLEnum>& enums) {
        std::vector<cpp::Enum> es;

        for (const auto& e : enums) {
            cpp::Enum em(e.name, wire_to_type(wire_type::INT));
            for (const auto& entry : e.entries) {
                em.add(cpp::Enum::Entity(entry.name, entry.value));
            }
        }

        return es;
    }

    std::vector<cpp::Method> Builder::gen_events(const std::vector<WLEvent>& events) {
        std::vector<cpp::Method> methods;

        for (const auto& event : events) {
            cpp::Method m(event.name, void_type);
            m.append_parameters(gen_parameters(event.arguments));
            methods.push_back(m);
        }

        return methods;
    }

    std::vector<cpp::SimpleDeclaration> Builder::gen_variables(const WLInterface& interface) {
        std::vector<cpp::SimpleDeclaration> sds;
        static cpp::Type op_code_type("wire_op_t");
        int counter = 0;

        for (const auto& req : interface.requests) {
            sds.push_back(cpp::SimpleDeclaration(op_code_type, op_code_name(req.name), std::format(" = {:#04x}", counter++)));
        }

        counter = 0;
        for (const auto& ev : interface.events) {
            sds.push_back(cpp::SimpleDeclaration(op_code_type, op_code_name(ev.name), std::format(" = {:#04x}", counter++)));
        }

        return sds;
    }
}
