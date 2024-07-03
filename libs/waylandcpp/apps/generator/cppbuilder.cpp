#include <cctype>

#include "cppbuilder.hpp"
#include "cpp/declaration/simple_declarator.hpp"
#include "cpp/comments.hpp"
#include "cpp/declaration/enum.hpp"
#include "cpp/declaration/namespace.hpp"

namespace wayland::generator {

    static cpp::unqid_t ns_prefix("wire");

    inline bool is_essential(const std::string& interface) {
        return interface == "wl_display" || interface == "wl_registry";
    }

    std::string op_code_name(const std::string& orig) {
        return std::format("{}_op", orig);
    }

    std::string alias_type(const std::string& name) {
        return std::format("{}_cb_t", name);
    }

    std::string alias_name(const std::string& name) {
        return std::format("{}_cb", name);
    }

    cpp::specifier_t Builder::wire_to_type(wire_type type) {
        switch (type) {
            case wire_type::FD:
                static cpp::qualified_id_t wire_fd_type(ns_prefix, cpp::unqid_t("wire_fd_t"));
                return wire_fd_type;
                break;
            case wire_type::FIXED:
                static cpp::qualified_id_t wire_fixed_type(ns_prefix, cpp::unqid_t("wire_fixed_t"));
                return wire_fixed_type;
                break;
            case wire_type::INT:
                static cpp::qualified_id_t wire_int_type(ns_prefix, cpp::unqid_t("wire_int_t"));
                return wire_int_type;
                break;
            case wire_type::UINT:
                static cpp::qualified_id_t wire_uint_type(ns_prefix, cpp::unqid_t("wire_uint_t"));
                return wire_uint_type;
                break;
            case wire_type::NEW_ID:
                static cpp::qualified_id_t wire_new_id_type(ns_prefix, cpp::unqid_t("wire_new_id_t"));
                return wire_new_id_type;
                break;
            case wire_type::OBJECT:
                static cpp::qualified_id_t wire_object_type(ns_prefix, cpp::unqid_t("wire_object_id_t"));
                return wire_object_type;
                break;
            case wire_type::ARRAY:
                static cpp::qualified_id_t wire_array_type(ns_prefix, cpp::unqid_t("wire_array_t"));
                return wire_array_type;
                break;
            case wire_type::STRING:
                static cpp::qualified_id_t wire_string_type(ns_prefix, cpp::unqid_t("wire_string_t"));
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


        cpp::clas cl(interface.name);
        cl.append(cpp::public_access);

        for (auto& cb_type : gen_callback_types(interface.events)) {
            cl.append(cb_type);
        }

        auto sock = cpp::unqid_t("sock");
        auto id = cpp::unqid_t("id");

        for (auto& ctr : gen_constructors(interface)) {
            cl.append(ctr);
        }

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
        cl.append(gen_dispatcher(interface.events));

        cl.append(cpp::protected_access);
        for (auto& var : gen_variables(interface)) {
            cl.append(var);
        }

        cpp::namespace_t ns("waylandcpp::interface");
        ns.push_back(cl);

        text << ns;

        return text;
    }

    cpp::compound_statement_t Builder::gen_request_body(const cpp::function_declaration_t& req) {
        auto shared_expr = [](auto s) { return std::make_shared<cpp::expression_statement_t>(s); };
        cpp::compound_statement_t body;
        auto op_name = op_code_name(std::get<cpp::unqid_t>(req.noptr_decl()).id());
        auto object_builder_def = shared_expr(std::format("wire::WireObjectBuilder builder(id, {})", op_name).c_str());
        static const auto write = shared_expr("sock.write(builder.data(), builder.size())");

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
            cpp::enum_specifier_t em(e.name);
            for (const auto& entry : e.entries) {
                auto name = entry.name;
                if (std::isdigit(name[0])) {
                    name = "_" + name;
                }
                auto value = entry.value;
                em.add(cpp::enum_specifier_t::entity_t(name, value));
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
            cpp::compound_statement_t body;
            {
                auto callback_name = alias_name(event.name);
                std::stringstream call_ss;
                call_ss << callback_name << "(";
                if (event.arguments.size() > 0) {
                    auto arg = event.arguments.begin();
                    call_ss << arg->name;
                    while (++arg != event.arguments.end()) {
                        call_ss << ", " << arg->name;
                    }
                }
                call_ss << ")";
                cpp::statement_ptr call_callback = std::make_shared<cpp::expression_statement_t>(call_ss.str());
                body.push_back(std::make_shared<cpp::if_statement_t>(cpp::condition_t(callback_name), call_callback));
            }
            methods.emplace_back(ds, event_decl, body);
        }

        return methods;
    }


    std::vector<cpp::simple_declaration_t> Builder::gen_variables(const WLInterface& interface) {
        std::vector<cpp::simple_declaration_t> sds;
        auto ns = cpp::unqid_t(ns_prefix);
        static cpp::qid_t op_code_type(ns, cpp::unqid_t("wire_op_t"));
        int counter = 0;

        auto add_field = [&] (auto type, auto id) {
            cpp::decl_specifier_seq_t decl_spec(type);
            cpp::init_declarator_list_t init_decl(id);
            sds.emplace_back(decl_spec, init_decl);
        };

        for (const auto& req : interface.requests) {
            cpp::decl_specifier_seq_t decl_spec(cpp::static_specifier, cpp::constexpr_specifier, op_code_type);
            cpp::init_declarator_list_t init_decl({cpp::unqid_t(op_code_name(req.name)), std::format("{:#04x}", counter++)});
            sds.emplace_back(decl_spec, init_decl);
        }

        counter = 0;
        for (const auto& ev : interface.events) {
            cpp::decl_specifier_seq_t decl_spec(cpp::static_specifier, cpp::constexpr_specifier, op_code_type);
            cpp::init_declarator_list_t init_decl({cpp::unqid_t(op_code_name(ev.name)), std::format("{:#04x}", counter++)});
            sds.emplace_back(decl_spec, init_decl);
        }

        add_field(socket_type, cpp::lval_t(cpp::unqid_t("sock")));
        add_field(wire_to_type(wire_type::OBJECT), cpp::unqid_t("id"));

        if (!is_essential(interface.name)) {
            add_field(wire_to_type(wire_type::UINT), cpp::unqid_t("name"));
        }

        for (const auto& ev : interface.events) {
            add_field(cpp::unqid_t(alias_type(ev.name)), cpp::unqid_t(alias_name(ev.name)));
        }

        return sds;
    }

    cpp::function_t Builder::gen_dispatcher(const std::vector<WLEvent>& events) {
        cpp::decl_specifier_seq_t ds(void_type);

        cpp::parameter_list_t params;
        {
            auto ns = cpp::unqid_t(ns_prefix);
            static cpp::qid_t op_code_type(ns, cpp::unqid_t("wire_op_t"));
            cpp::decl_specifier_seq_t op_type(op_code_type);
            cpp::init_declarator_t    op_decl(cpp::unqid_t("op"));
            params.emplace_back(op_type, op_decl);

            auto wbp = cpp::unqid_t("wire::WireBufferParser");
            cpp::decl_specifier_seq_t body_type(wbp);
            cpp::init_declarator_t    body(cpp::lval_t(cpp::unqid_t("body")));
            params.emplace_back(body_type, body);
        }
        cpp::function_declaration_t req_decl(cpp::unqid_t("dispatch"), params);

        auto switch_body = std::make_shared<cpp::compound_statement_t>();
        for (auto& event : events) {
            switch_body->push_back(std::make_shared<cpp::label_statement_t>(cpp::label_statement_t::label_t::CASE, op_code_name(event.name)));
            auto case_body = std::make_shared<cpp::compound_statement_t>();

            std::stringstream auto_;
            std::stringstream parse;
            std::stringstream call;
            call << event.name << "(";
            if (event.arguments.size() > 0) {
                auto_ << "auto [";
                parse << "body.parse<";
                auto it = event.arguments.begin();
                auto add_new = [&](auto arg) {
                    // Doing it like this for now because enum, function and parameters can have same name in xml.
                    auto value_name = arg.name + "_v";
                    auto_ << value_name;
                    auto type = std::get<cpp::qid_t>(wire_to_type(arg.type));
                    parse << ns_prefix.id() << "::" << type.id().id();
                    call << value_name;
                };
                add_new(*it);

                while (++it != event.arguments.end()) {
                    auto_ << ",";
                    parse << ",";
                    call  << ",";
                    add_new(*it);
                }
                parse << ">()";
                auto_ << "] = " << parse.str();
                case_body->push_back(std::make_shared<cpp::expression_statement_t>(auto_.str()));
            }
            call << ")";
            case_body->push_back(std::make_shared<cpp::expression_statement_t>(call.str()));
            switch_body->push_back(case_body);
            switch_body->push_back(std::make_shared<cpp::jump_statement_t>(cpp::jump_statement_t::jump_t::BREAK));
        }
        auto dispatch_switch = std::make_shared<cpp::switch_statement_t>(cpp::expression_t("op"), switch_body);

        cpp::compound_statement_t function_body;
        function_body.push_back(dispatch_switch);
        auto body = cpp::function_body_t(function_body);

        cpp::function_t dispatch_f(ds, req_decl, body);
        return dispatch_f;
    }

    std::vector<std::string> Builder::get_types_list(const std::vector<WLArgument>& arguments) {
        std::vector<std::string> types;
        for (auto arg : arguments) {
            auto type = std::get<cpp::qid_t>(wire_to_type(arg.type));
            types.emplace_back(std::format("{}::{}", ns_prefix.id(), type.id().id()));
        }
        return types;
    }

    std::vector<cpp::type_alias_t> Builder::gen_callback_types(const std::vector<WLEvent>& events) {
        std::vector<cpp::type_alias_t> aliases;
        for (auto& event : events) {
            std::string type_id;
            std::stringstream ss;
            if (event.arguments.size() > 0) {
                auto type_list = get_types_list(event.arguments);
                auto tl_id = type_list.begin();
                ss << *tl_id;
                while (++tl_id < type_list.end()) {
                    ss << ", " << *tl_id;
                }
            }
            type_id = std::format("callback_t<{}>", ss.str());
            aliases.emplace_back(alias_type(event.name), type_id);
        }
        return aliases;
    }

    std::vector<cpp::function_t> Builder::gen_constructors(const WLInterface& interface) {
        std::vector<cpp::function_t> ctrs;

        auto sock = cpp::unqid_t("sock");
        auto id = cpp::unqid_t("id");

        auto socket_param = cpp::parameter_t(socket_type, cpp::lval_t(sock));
        auto id_param = cpp::parameter_t(wire_to_type(wire_type::OBJECT), cpp::init_declarator_t(id, cpp::copy_initialization_t("0x0"))); 

        cpp::parameter_list_t ctr_params ({socket_param, id_param});
        cpp::ctor_initializer_t ctor_init;
        for (auto& m : { sock, id }) {
            ctor_init.elements().emplace_back(m, m.id());
        }

        if (!is_essential(interface.name)) {
            auto name = cpp::unqid_t("name");
            ctr_params.emplace_back(wire_to_type(wire_type::UINT), cpp::init_declarator_t(cpp::rval_t(name), cpp::copy_initialization_t("0x0")));
            ctor_init.elements().emplace_back(name, name.id());
        }

        cpp::function_body_t ctr_body({}, ctor_init);
        auto ctr_decl = cpp::function_declaration_t(cpp::unqid_t(interface.name), ctr_params);
        ctrs.emplace_back(ctr_decl, ctr_body);


        id_param = cpp::parameter_t(wire_to_type(wire_type::OBJECT), id); 
        cpp::parameter_list_t ctr2_params({socket_param, id_param});

        for (const auto& ev : interface.events) {
            auto uniq_name = cpp::unqid_t(alias_name(ev.name));
            auto n = cpp::rval_t(uniq_name);
            ctr2_params.emplace_back(cpp::unqid_t(alias_type(ev.name)), n);
            ctor_init.elements().emplace_back(uniq_name, alias_name(ev.name));
        }

        if (!is_essential(interface.name)) {
            auto name = cpp::unqid_t("name");
            ctr2_params.emplace_back(wire_to_type(wire_type::UINT), cpp::init_declarator_t(cpp::rval_t(name), cpp::copy_initialization_t("0x0")));
        }

        cpp::function_body_t ctr2_body({}, ctor_init);
        ctr_decl = cpp::function_declaration_t(cpp::unqid_t(interface.name), ctr2_params);
        ctrs.emplace_back(ctr_decl, ctr2_body);

        return ctrs;
    }
}
