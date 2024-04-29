#ifndef WAYLAND_GENERATOR_PROTOCOL_H
#define WAYLAND_GENERATOR_PROTOCOL_H
#include <string>
#include <vector>
#include <optional>

#include <boost/property_tree/ptree.hpp>

#include "wire_types.hpp"

namespace wayland::generator {
    enum class wire_type {
        NEW_ID,
        OBJECT,
        UINT,
        INT,
        FIXED,
        STRING,
        ARRAY,
        FD
    };

    struct WLDescription {
        static constexpr std::string xml_name = "description";
        std::string description;
        std::string summary;
        static WLDescription create_from_xml(const boost::property_tree::ptree& description);
    };

    struct WLArgument {
        static constexpr std::string xml_name = "arg";
        std::string name;
        wire_type type;
        std::string summary;
        std::optional<std::string> arg_enum;
        std::optional<std::string> arg_interface;
        static WLArgument create_from_xml(const boost::property_tree::ptree& description);
    };

    struct WLAction {
        std::string name;
        WLDescription description;
        std::vector<WLArgument> arguments;
    };

    struct WLRequest : public WLAction {
        static WLRequest create_from_xml(const boost::property_tree::ptree& description);
    };

    struct WLEvent : public WLAction {
        static WLEvent create_from_xml(const boost::property_tree::ptree& description);
    };

    struct WLEnum {
        static constexpr std::string xml_name = "enum";

        struct WLEntry {
            static constexpr std::string xml_name = "entry";
            std::string name;
            std::string value;
            std::optional<std::string> summary;
            static WLEntry create_from_xml(const boost::property_tree::ptree& description);
        };

        std::string name;
        bool bitfield = false;
        std::optional<WLDescription> description;
        std::vector<WLEntry> entries;
        static WLEnum create_from_xml(const boost::property_tree::ptree& description);
    };


    struct WLInterface {
        std::string             name;
        WLDescription           description;
        std::vector<WLRequest>  requests;
        std::vector<WLEvent>    events;
        std::vector<WLEnum>     enums;
        int                     version;

        static WLInterface create_from_xml(const boost::property_tree::ptree& description);
    };
}

#endif /* WAYLAND_GENERATOR_PROTOCOL_H */
