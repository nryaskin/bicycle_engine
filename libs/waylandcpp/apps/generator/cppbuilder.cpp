#include "cppbuilder.hpp"
#include "cpp/declaration/simple_declarator.hpp"
#include "cpp/comments.hpp"
#include "cpp/declaration/enum.hpp"

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
        text << language::newline;

        // cpp::Namespace ns("waylandcpp::interface");
        cpp::clas cl(interface.name);
        cl.append(cpp::public_access);

        auto sock = cpp::unqid_t("sock");
        auto id = cpp::unqid_t("id");
        auto name = cpp::unqid_t("name");

        auto socket_param = cpp::parameter_t(socket_type, cpp::rval_t(sock));
        auto id_param = cpp::parameter_t(wire_to_type(wire_type::OBJECT), cpp::init_declarator_t(id, cpp::copy_initialization_t("0x0"))); 
        auto name_param = cpp::parameter_t(string_type, cpp::init_declarator_t(cpp::rval_t(name), cpp::copy_initialization_t("\"\"")));
        cpp::parameter_list_t ctr_params ({socket_param, id_param, name_param});

        auto ctr_decl = cpp::function_declaration_t(cpp::unqid_t(interface.name), ctr_params);
        cpp::ctor_initializer_t ctor_init;
        for (auto& m : { sock, id, name }) {
            ctor_init.elements().emplace_back(m, m.id());
        }
        cpp::function_body_t ctr_body({}, ctor_init);
        auto ctr = cpp::function_t(ctr_decl, ctr_body);
        cl.append(ctr);

        for (auto& e : gen_enums(interface.enums)) {
            cl.append(e);
        }

        cl.append(language::comment_t("Requests"));
        for (auto& req : gen_requests(interface.requests)) {
            cl.append(req);
        }

        cl.append(language::comment_t("Events"));
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

    cpp::compound_statement_t Builder::gen_request_body(const cpp::function_declaration_t& req) {
        auto shared_expr = [](auto s) { return std::make_shared<cpp::expression_statement_t>(s); };
        cpp::compound_statement_t body;
        auto op_name = op_code_name(std::get<cpp::unqid_t>(req.noptr_decl()).id());
        auto object_builder_def = shared_expr(std::format("waylandcpp::wire::WireObjectBuilder builder(id_, {})", op_name).c_str());
        static const auto write = shared_expr("s_.write(builder.data(), builder.size())");

        body.push_back(object_builder_def);

        if (req.param_list().size() > 0) {
            std::stringstream ss;
            ss << "builder.add(";
            auto pl = req.param_list();
            auto it = pl.begin();
            ss << std::get<cpp::unqid_t>(it->init_decl().declarator()).id();
            while (++it != pl.end()) {
                auto name = std::get<cpp::unqid_t>(it->init_decl().declarator()).id();
                ss << ", " << name;
            }
            //bool comma = false;
            //for (const auto& p : req.param_list()) {
            //    ss << (comma? ", " : "") << p.name();
            //    comma = true;
            //}
            ss << ")";
            body.push_back(shared_expr(ss.str()));
        }

        body.push_back(write);

        return body;
    }

    cpp::parameter_list_t Builder::gen_parameters(const std::vector<WLArgument>& args) {
        cpp::parameter_list_t params;

        for (const auto& arg : args) {
            cpp::decl_specifier_seq_t ds(wire_to_type(arg.type));
            cpp::init_declarator_t indecl(cpp::unqid_t(arg.name));
            // NOTE: I am doing basic implementation here so I am not going to use enums in case there were enums specified in xml, but I am going to do it latter to have links between types in events and enums.
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
            auto body = cpp::function_body_t(gen_request_body(req_decl));
            cpp::function_t function(ds, req_decl, body);
            methods.push_back(function);
        }

        return methods;
    }

    std::vector<cpp::enum_specifier_t> Builder::gen_enums(const std::vector<WLEnum>& enums) {
        std::vector<cpp::enum_specifier_t> es;

        for (const auto& e : enums) {
            cpp::enum_specifier_t em(e.name, wire_to_type(wire_type::INT));
            for (const auto& entry : e.entries) {
                em.add(cpp::enum_specifier_t::entity_t(entry.name, entry.value));
            }
            es.push_back(em);
        }

        return es;
    }

    std::vector<cpp::function_t> Builder::gen_events(const std::vector<WLEvent>& events) {
        std::vector<cpp::function_t> methods;

        for (const auto& event : events) {
            cpp::decl_specifier_seq_t ds({ cpp::virtual_qualifier, void_type });
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
            cpp::decl_specifier_seq_t decl_spec(cpp::static_specifier, cpp::constexpr_specifier, op_code_type.id().id());
            cpp::init_declarator_list_t init_decl({cpp::unqid_t(op_code_name(req.name)), std::format("{:#04x}", counter++)});
            sds.emplace_back(decl_spec, init_decl);
        }

        counter = 0;
        for (const auto& ev : interface.events) {
            cpp::decl_specifier_seq_t decl_spec(cpp::static_specifier, cpp::constexpr_specifier, op_code_type.id().id());
            cpp::init_declarator_list_t init_decl({cpp::unqid_t(op_code_name(ev.name)), std::format("{:#04x}", counter++)});
            sds.emplace_back(decl_spec, init_decl);
        }

        {
            cpp::decl_specifier_seq_t decl_spec(socket_type);
            cpp::init_declarator_list_t init_decl(cpp::lval_t(cpp::unqid_t("sock")));
            sds.emplace_back(decl_spec, init_decl);
        }

        return sds;
    }
}
