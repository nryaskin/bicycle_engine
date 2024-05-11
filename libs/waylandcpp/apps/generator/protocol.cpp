#include "protocol.hpp"
#include <iostream>
#include <format>

namespace wayland::generator {
    template<typename T>
    static T get_property(const boost::property_tree::ptree& node, std::string&& name) {
        return node.get<T>(std::string("<xmlattr>.") + name);
    }

    bool prop_exist(const boost::property_tree::ptree& node, std::string&& name) {
        const auto& xml_attr = node.get_child("<xmlattr>");
        return xml_attr.find(name) != xml_attr.not_found();
    }

    static wire_type str_to_wire(std::string type_str) {
        wire_type type;
        if (type_str == "new_id") {
            type = wire_type::NEW_ID;
        } else if (type_str == "object") {
            type = wire_type::OBJECT;
        } else if (type_str == "uint") {
            type = wire_type::UINT;
        } else if (type_str == "int") {
            type = wire_type::INT;
        } else if (type_str == "string") {
            type = wire_type::STRING;
        } else if (type_str == "fixed") {
            type = wire_type::FIXED;
        } else if (type_str == "fd") {
            type = wire_type::FD;
        } else if (type_str == "array") {
            type = wire_type::ARRAY;
        } else if (type_str == "destructor") {
            std::cout << "DON'T KNOW WHY THIS TYPE IS DESTRUCTOR IN XML" << std::endl;
            type = wire_type::UINT;
        } else {
            assert(false);
        }
        return type;
    }

    WLInterface WLInterface::create_from_xml(const boost::property_tree::ptree& description) {
        WLInterface interface;
        interface.name = get_property<std::string>(description, "name");
        interface.version = get_property<int>(description, "version");
        auto descr_xml = description.get_child("description");
        interface.description = WLDescription::create_from_xml(descr_xml);
        for (const auto& child : description) {
            if (child.first == "request") {
                interface.requests.push_back(WLRequest::create_from_xml(child.second));
            }

            if (child.first == "event") {
                interface.events.push_back(WLEvent::create_from_xml(child.second));
            }

            if (child.first == "enum") {
                interface.enums.push_back(WLEnum::create_from_xml(child.second));
            }
        }

        return interface;
    }

    WLRequest WLRequest::create_from_xml(const boost::property_tree::ptree& description) {
        WLRequest request;
        request.name = get_property<std::string>(description, "name");
        for (const auto& node : description) {
            if (node.first == WLArgument::xml_name) {
                request.arguments.push_back(WLArgument::create_from_xml(node.second));
            }
            if (node.first == WLDescription::xml_name) {
                request.description = WLDescription::create_from_xml(node.second);
            }
        }

        return request;
    }

    WLArgument WLArgument::create_from_xml(const boost::property_tree::ptree& description) {
        WLArgument argument;
        argument.name = get_property<std::string>(description, "name");
        argument.summary = get_property<std::string>(description, "summary");
        argument.type = str_to_wire(get_property<std::string>(description, "type"));

        if (prop_exist(description, "interface")) {
            argument.arg_interface = get_property<std::string>(description, "interface");
        }

        if (prop_exist(description, "enum")) {
            argument.arg_enum = get_property<std::string>(description, "enum");
        }

        return argument;
    }

    WLDescription WLDescription::create_from_xml(const boost::property_tree::ptree& description) {
        return {
            .description = description.data(),
            .summary = get_property<std::string>(description, "summary"),
        };
    }

    WLEvent WLEvent::create_from_xml(const boost::property_tree::ptree& description) {
        WLEvent event;

        event.name = get_property<std::string>(description, "name");
        for (const auto& elems: description) {
            if (elems.first == WLDescription::xml_name) {
                event.description = WLDescription::create_from_xml(elems.second);
            } else if (elems.first == WLArgument::xml_name) {
                event.arguments.push_back(WLArgument::create_from_xml(elems.second));
            }
        }
        
        return event;
    }

    WLEnum WLEnum::create_from_xml(const boost::property_tree::ptree& description) {
        WLEnum wlenum;
        wlenum.name = get_property<std::string>(description, "name");
        if (prop_exist(description, "bitfield")) {
            wlenum.bitfield = get_property<bool>(description, "bitfield");
        }
        for (const auto& item: description) {
            if (item.first == WLDescription::xml_name) {
                wlenum.description = WLDescription::create_from_xml(item.second);
            }
            if (item.first == WLEntry::xml_name) {
                wlenum.entries.push_back(WLEntry::create_from_xml(item.second));
            }
        }

        return wlenum;
    }

    WLEnum::WLEntry WLEnum::WLEntry::create_from_xml(const boost::property_tree::ptree& description) {
        WLEntry entry;
        entry.name = get_property<std::string>(description, "name");
        if (prop_exist(description, "summary")) {
            entry.summary = get_property<std::string>(description, "summary");
        }
        entry.value = get_property<std::string>(description, "value");

        return entry;
    }
}
