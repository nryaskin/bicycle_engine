#include "cppbuilder.hpp"
#include "cpp/declaration/simple_declarator.hpp"

namespace wayland::generator {

    std::string ns_prefix = "waylandcpp::wire::";

    std::string op_code_name(const std::string& orig) {
        return std::format("{}_op", orig);
    }

    cpp::simple_type_specifier_t Builder::wire_to_type(wire_type type) {
        switch (type) {
            case wire_type::FD:
                static cpp::simple_type_specifier_t wire_fd_type(ns_prefix + "wire_fd_t");
                return wire_fd_type;
                break;
            case wire_type::FIXED:
                static cpp::simple_type_specifier_t wire_fixed_type(ns_prefix + "wire_fixed_t");
                return wire_fixed_type;
                break;
            case wire_type::INT:
                static cpp::simple_type_specifier_t wire_int_type(ns_prefix +"wire_int_t");
                return wire_int_type;
                break;
            case wire_type::UINT:
                static cpp::simple_type_specifier_t wire_uint_type(ns_prefix + "wire_uint_t");
                return wire_uint_type;
                break;
            case wire_type::NEW_ID:
                static cpp::simple_type_specifier_t wire_new_id_type(ns_prefix + "wire_new_id_t");
                return wire_new_id_type;
                break;
            case wire_type::OBJECT:
                static cpp::simple_type_specifier_t wire_object_type(ns_prefix + "wire_object_id_t");
                return wire_object_type;
                break;
            case wire_type::ARRAY:
                static cpp::simple_type_specifier_t wire_array_type(ns_prefix + "wire_array_t");
                return wire_array_type;
                break;
            case wire_type::STRING:
                static cpp::simple_type_specifier_t wire_string_type(ns_prefix + "wire_string_t");
                return wire_string_type;
                break;
        }
        return void_type;
    }

    cpp::formatter::text Builder::build(const wayland::generator::WLInterface& interface) {
        cpp::formatter::text text;

        for (auto& include : common_includes) {
            text << include;
            text << language::newline;
        }

        // cpp::Namespace ns("waylandcpp::interface");
        cpp::clas cl(interface.name);
        cl.append(cpp::public_access);

        auto sock = cpp::unqid_t("sock");
        auto id = cpp::unqid_t("id");
        auto name = cpp::unqid_t("name");

        auto socket_param = cpp::parameter_t(socket_type, cpp::rval_t(sock));
        auto id_param = cpp::parameter_t(wire_to_type(wire_type::OBJECT), cpp::init_declarator_t(id, cpp::copy_initialization_t("0x0"))); 
        auto name_param = cpp::parameter_t(string_type, cpp::init_declarator_t(cpp::rval_t(name), cpp::copy_initialization_t("\"\"")));
        cpp::parameter_list_t ctr_params;
        ctr_params.push_back(socket_param);
        ctr_params.push_back(id_param);
        ctr_params.push_back(name_param);

        auto ctr_decl = cpp::function_declaration_t(cpp::unqid_t(interface.name), ctr_params);
        cpp::ctor_initializer_t ctor_init;
        for (auto& m : { sock, id, name }) {
            ctor_init.elements().emplace_back(m, m.id());
        }
        cpp::function_body_t ctr_body({}, ctor_init);
        auto ctr = cpp::function_t(ctr_decl, ctr_body);
        cl.append(ctr);

        //auto enums = gen_enums(interface.enums);
        //for (auto& e : enums) {
        //    cl.add(e);
        //}

        for (auto& req : gen_requests(interface.requests)) {
            cl.append(req);
        }

        for (auto& event : gen_events(interface.events)) {
            cl.append(event);
        }

        cl.append(cpp::protected_access);
        for (auto& var : gen_variables(interface)) {
            cl.append(var);
        }

        text << cl;

        return text;
    }

    //cpp::MethodBody Builder::gen_request_body(const cpp::Method& req) {
    //     const std::string object_builder_def = std::format("waylandcpp::wire::WireObjectBuilder builder(id_, {});", op_code_name(req.name()));
    //     static const std::string write = "s_.write(builder.data(), builder.size());";
    //     cpp::MethodBody mb;

    //     mb.add(object_builder_def);

    //     if (req.get_params().size() > 0) {
    //         std::stringstream ss;
    //         ss << "builder.add(";
    //         bool comma = false;
    //         for (const auto& p : req.get_params()) {
    //             ss << (comma? ", " : "") << p.name();
    //             comma = true;
    //         }
    //         ss << ");";
    //         mb.add(ss.str());
    //     }

    //     mb.add(write);

    //     return mb;
    //}
    //cpp::Definition Builder::gen_definition(const cpp::Class& cs, const cpp::Class::Ctr& ctr) {
    //    return cpp::Definition(cs, ctr, gen_ctr_body(ctr));
    //}

    cpp::parameter_list_t Builder::gen_parameters(const std::vector<WLArgument>& args) {
        cpp::parameter_list_t params;

        for (const auto& arg : args) {
            cpp::decl_specifier_seq_t ds(wire_to_type(arg.type));
            cpp::init_declarator_t indecl(cpp::unqid_t(arg.name));
            // NOTE: I am doing basic implementation here so I am not going to user enums in case there were enums specified in xml, but I am going to do it latter to have links between types in events and enums.
            params.emplace_back(ds, indecl);
        }

        return params;
    }

    std::vector<cpp::function_t> Builder::gen_requests(const std::vector<WLRequest>& requests) {
        std::vector<cpp::function_t> methods;

        for (const auto& req : requests) {
            cpp::decl_specifier_seq_t ds(void_type);
            auto req_params = gen_parameters(req.arguments);
            cpp::function_declaration_t req_decl(cpp::unqid_t(req.name), req_params);
            auto body = cpp::function_body_t({});
            cpp::function_t function(ds, req_decl, body);
            methods.push_back(function);
        }

        return methods;
    }

    //std::vector<cpp::Enum> Builder::gen_enums(const std::vector<WLEnum>& enums) {
    //    std::vector<cpp::Enum> es;

    //    for (const auto& e : enums) {
    //        cpp::Enum em(e.name, wire_to_type(wire_type::INT));
    //        for (const auto& entry : e.entries) {
    //            em.add(cpp::Enum::Entity(entry.name, entry.value));
    //        }
    //    }

    //    return es;
    //}

    std::vector<cpp::function_t> Builder::gen_events(const std::vector<WLEvent>& events) {
        std::vector<cpp::function_t> methods;

        for (const auto& event : events) {
            cpp::decl_specifier_seq_t ds(void_type);
            auto event_params = gen_parameters(event.arguments);
            cpp::function_declaration_t event_decl(cpp::unqid_t(event.name), event_params);
            auto body = cpp::function_body_t({});
            cpp::function_t function(ds, event_decl, body);
            methods.push_back(function);
        }

        return methods;
    }

    std::vector<cpp::simple_declaration_t> Builder::gen_variables(const WLInterface& interface) {
        std::vector<cpp::simple_declaration_t> sds;
        auto ns = cpp::unqid_t(ns_prefix);
        static cpp::qualified_id_t op_code_type(ns, cpp::unqid_t("wire_op_t"));
        int counter = 0;

        for (const auto& req : interface.requests) {
            cpp::init_declarator_list_t init_decl({cpp::unqid_t(op_code_name(req.name)), std::format("{:#04x}", counter++)});
            cpp::decl_specifier_seq_t decl_spec(cpp::static_specifier, cpp::constexpr_specifier, op_code_type.id().id());
            sds.emplace_back(decl_spec, init_decl);
        }

        counter = 0;
        for (const auto& ev : interface.events) {
            cpp::init_declarator_list_t init_decl({cpp::unqid_t(op_code_name(ev.name)), std::format("{:#04x}", counter++)});
            cpp::decl_specifier_seq_t decl_spec(cpp::static_specifier, cpp::constexpr_specifier, op_code_type.id().id());
            sds.emplace_back(decl_spec, init_decl);
        }

        return sds;
    }
}
