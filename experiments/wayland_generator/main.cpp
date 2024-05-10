#include <string>
#include <iostream>
#include <sstream>
#include <format>

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>

#include "wire_types.hpp"
#include "protocol.hpp"
#include "wlstream.hpp"
#include "cppbuilder.hpp"

// CPP
#include "cpp/file.hpp"
#include "cpp/type.hpp"
#include "cpp/namespace.hpp"
#include "cpp/class.hpp"
#include "cpp/enum.hpp"
#include "cpp/document.hpp"
#include "cpp/formatter.hpp"


namespace pt = boost::property_tree;

static const std::string wayland_xml_path = "/usr/share/wayland/wayland.xml";

namespace wg = wayland::generator;


int main1 () {
/**
 * 
 * class WLDisplay {
 *     wire_object_id_t id = 1;
 *     wire_op_t sync_op = 0x0000;
 *     wire_op_t get_registry_op = 0x0001;
 * public:
 *     enum class error : wire_uint_t {
 *         INVALID_OBJECT = 0,
 *         INVALID_METHOD = 1,
 *         NO_MEMORY = 2,
 *         IMPLEMENTATION = 3
 *     }; * 
 * 
 * 
 *     // Methods
 *     void get_registry(wire_new_id_t registry_id); 
 *     void sync(wire_new_id_t callback_id);
 *     // Events
 *     void error(wire_object_id_t id, wire_uint_t code, std::string& msg);
 *     void delete_id(wire_object_id_t id);
 * 
 * private:
 *     WLSocket& s_;
 * };
 *
 */

    cpp::Type wire_object_id_type("wire_object_id_t");
    cpp::Type wire_op_type("wire_op_t");
    cpp::Type wire_uint_type("wire_uint_t");
    cpp::Type void_type("void");
    cpp::Type wire_new_id_type("wire_new_id_t");
    cpp::Type sock_type("WLSocket");
    cpp::Ref  sock_type_ref(sock_type);
    cpp::Type string_type("std::string");
    cpp::RRef string_type_rref(string_type);

    cpp::Header display_header("display");
    cpp::QuoteInclusion wire_include;
    wire_include.file = "wire_types.hpp";

    display_header.add_include(wire_include);
    cpp::Namespace ns("wayland");
    cpp::Class cl("Display");
    cpp::Class::Ctr display_ctr;
    cpp::Parameter socket_param(sock_type_ref, "s");
    cpp::Parameter name_param(string_type_rref, "name", "= \"\"");
    display_ctr.add(socket_param);
    display_ctr.add(name_param);
    cl.add_ctr(display_ctr);

    cpp::SimpleDeclaration id(wire_object_id_type, "id_", "= 1");
    cpp::SimpleDeclaration sync_op(wire_op_type, "sync_op", "= 0x0000");
    cpp::SimpleDeclaration get_reg_op(wire_op_type, "get_registry_op", "= 0x0001");

    cl.add(id);
    cl.add(sync_op);
    cl.add(get_reg_op);
    cl.add(cpp::AccessModifier(cpp::access_modifier_t::PUBLIC));

    cpp::Enum error_enum("error", wire_uint_type);
    error_enum.add(cpp::Enum::Entity("INVALID_OBJECT", "0"));
    error_enum.add(cpp::Enum::Entity("INVALID_METHOD", "1"));
    error_enum.add(cpp::Enum::Entity("NO_MEMORY", "2"));
    error_enum.add(cpp::Enum::Entity("IMPLEMENTATION", "3"));

    cl.add(error_enum);

    //  void get_registry(wire_new_id_t registry_id); 
    cpp::Method get_reg_method("get_registry", void_type);
    cpp::Parameter reg_id(wire_new_id_type, "registry_id");
    get_reg_method.add_parameter(reg_id);
    cl.add(get_reg_method);
    cl.add(cpp::AccessModifier(cpp::access_modifier_t::PRIVATE));
    cpp::SimpleDeclaration socket_field(sock_type_ref, "s");
    cl.add(socket_field);

    ns.add(cl);
    display_header.add(ns);

    cpp::Source display_source("display");
    display_source.set_namespace(ns);
    cpp::MethodBody reg_meth_body;
    reg_meth_body.add("WireObjectBuilder builder(id, get_registry_op);");
    reg_meth_body.add("builder.add(registry_id);");
    reg_meth_body.add("s_.write(builder.data(), builder.size());");
    cpp::Definition get_reg_def(cl, get_reg_method, reg_meth_body);
    display_source.add(get_reg_def);

    cpp::Document header(display_header.get_name(), 80);
    header << display_header;

    cpp::Document source(display_source.get_name(), 80);
    source << display_source;

    std::filesystem::path tmp = "/home/tutturu/projects/bicycle_engine/tmp";
    if(!std::filesystem::exists(tmp)) {
        throw std::runtime_error("Output dir doesn't exist!");
    }
    header.save(tmp);
    source.save(tmp);

    return 0;
}

int main() {
    std::vector<wg::WLInterface> interfaces;
    pt::ptree tree;

    pt::read_xml(wayland_xml_path, tree);
    auto xml_interfaces = tree.get_child("protocol");

    // Q: Why this form sometimes doesn't work?
    auto protocol_name = tree.get_child("protocol.<xmlattr>.name").data();
    std::cout << "Protocol name: " << protocol_name << std::endl;
    for (auto& interface: xml_interfaces) {
        if (interface.first == "interface") {
            interfaces.push_back(wg::WLInterface::create_from_xml(interface.second));
        }
    }

    for (auto& interface: interfaces) {
        std::cout << interface << std::endl;
    }

    cpp::Includes includes;
    cpp::QuoteInclusion wire_include("wire_types.hpp");
    includes.add(wire_include);
    cpp::AngleInclusion string_include("string");
    cpp::AngleInclusion vector_include("vector");
    includes.add(string_include);
    includes.add(vector_include);

    wayland::generator::Builder builder(includes);

    std::filesystem::path tmp = "/home/tutturu/projects/bicycle_engine/tmp";
    if(!std::filesystem::exists(tmp)) {
        throw std::runtime_error("Output dir doesn't exist!");
    }

    for (const auto& interface : interfaces) {
        auto [header, source] = builder.build(interface);

        cpp::Document hfile(header.get_name(), 80);
        hfile << header;

        cpp::Document cppfile(source.get_name(), 80);
        cppfile << source;

        hfile.save(tmp);
        cppfile.save(tmp);
    }


    return 0;
}
