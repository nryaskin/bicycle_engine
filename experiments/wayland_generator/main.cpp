#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <string>
#include <iostream>
#include <sstream>
#include <format>
#include "wire_types.hpp"
#include "protocol.hpp"
#include "wlstream.hpp"

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
    return 0;
}
