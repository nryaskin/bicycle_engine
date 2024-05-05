#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <string>
#include <iostream>
#include <sstream>
#include <format>
#include "wire_types.hpp"
#include "protocol.hpp"
#include "wlstream.hpp"

// CPP
#include "cpp/file.hpp"
#include "cpp/type.hpp"
#include "cpp/namespace.hpp"
#include "cpp/class.hpp"

namespace pt = boost::property_tree;

static const std::string wayland_xml_path = "/usr/share/wayland/wayland.xml";
/**
 *
 *<!ELEMENT protocol (copyright?, description?, interface+)>
 *  <!ATTLIST protocol name CDATA #REQUIRED>
 *<!ELEMENT copyright (#PCDATA)>
 *<!ELEMENT interface (description?,(request|event|enum)+)>
 *  <!ATTLIST interface name CDATA #REQUIRED>
 *  <!ATTLIST interface version CDATA #REQUIRED>
 *<!ELEMENT request (description?,arg*)>
 *  <!ATTLIST request name CDATA #REQUIRED>
 *  <!ATTLIST request type CDATA #IMPLIED>
 *  <!ATTLIST request since CDATA #IMPLIED>
 *<!ELEMENT event (description?,arg*)>
 *  <!ATTLIST event name CDATA #REQUIRED>
 *  <!ATTLIST event type CDATA #IMPLIED>
 *  <!ATTLIST event since CDATA #IMPLIED>
 *<!ELEMENT enum (description?,entry*)>
 *  <!ATTLIST enum name CDATA #REQUIRED>
 *  <!ATTLIST enum since CDATA #IMPLIED>
 *  <!ATTLIST enum bitfield CDATA #IMPLIED>
 *<!ELEMENT entry (description?)>
 *  <!ATTLIST entry name CDATA #REQUIRED>
 *  <!ATTLIST entry value CDATA #REQUIRED>
 *  <!ATTLIST entry summary CDATA #IMPLIED>
 *  <!ATTLIST entry since CDATA #IMPLIED>
 *<!ELEMENT arg (description?)>
 *  <!ATTLIST arg name CDATA #REQUIRED>
 *  <!ATTLIST arg type CDATA #REQUIRED>
 *  <!ATTLIST arg summary CDATA #IMPLIED>
 *  <!ATTLIST arg interface CDATA #IMPLIED>
 *  <!ATTLIST arg allow-null CDATA #IMPLIED>
 *  <!ATTLIST arg enum CDATA #IMPLIED>
 *<!ELEMENT description (#PCDATA)>
 *  <!ATTLIST description summary CDATA #REQUIRED>
 *
 *
 */

namespace wg = wayland::generator;


int main () {
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
 *     };
 * 
 *     WLDisplay(WLSocket& s, std::string&& name = "");
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
    cpp::Type void_type("void");
    cpp::Type wire_new_id_type("wire_new_id_t");
    cpp::Type sock_type("WLSocket");
    cpp::Ref  sock_type_ref(sock_type);
    cpp::Type string_type("std::string");
    cpp::RRef string_type_rref(string_type);

    cpp::Header cppfile("display");
    cpp::QuoteInclusion wire_include;
    wire_include.file = "wire_types.hpp";

    cppfile.add_include(wire_include);
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

    //  void get_registry(wire_new_id_t registry_id); 
    cpp::Method method("get_registry", void_type);
    cpp::Parameter reg_id(wire_new_id_type, "registry_id");
    method.add_parameter(reg_id);
    cl.add(method);
    cl.add(cpp::AccessModifier(cpp::access_modifier_t::PRIVATE));
    cpp::SimpleDeclaration socket_field(sock_type_ref, "s");
    cl.add(socket_field);

    ns.add(cl);
    cppfile.add(ns);

    std::cout << "File:\n" << cppfile.to_string() << "\nEnd File" << std::endl;

    return 0;
}

int main1() {
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

    return 0;
}
