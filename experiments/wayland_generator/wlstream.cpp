#include "wlstream.hpp"
#include <cassert>

namespace wg = wayland::generator;
using wire_type = wg::wire_type;
using WLArgument = wg::WLArgument;
using WLAction = wg::WLAction;
using WLInterface = wg::WLInterface;
using WLEnum = wg::WLEnum;
using WLEvent = wg::WLEvent;
using WLRequest = wg::WLRequest;

std::ostream& operator<<(std::ostream& os, const wire_type& type) {
    switch (type) {
        case wire_type::NEW_ID:
        {
            os << "NEW_ID";
        }
        break;
        case wire_type::OBJECT:
        {
            os << "OBJECT";
        }
        break;
        case wire_type::UINT:
        {
            os << "UINT";
        }
        break;
        case wire_type::INT:
        {
            os << "INT";
        }
        break;
        case wire_type::STRING:
        {
            os << "STRING";
        }
        break;
        case wire_type::ARRAY:
        {
            os << "ARRAY"; 
        }
        break;
        case wire_type::FD:
        {
            os << "FD";
        }
        break;
        case wire_type::FIXED:
        {
            os << "FIXED";
        }
        break;
        default:
        {
            os << "Unsupported" << std::endl;
            assert(false);
        }
    }
    return os;
}

std::ostream& operator<<(std::ostream& os, const WLArgument& argument) {
    os << "WLArgument: {\n";
    os << "argument: " << argument.name << "\n";
    os << "summary: " << argument.summary<< "\n";
    os << "type: " << argument.type << "\n";
    if (argument.arg_enum.has_value()) {
        os << "enum: " << argument.arg_enum.value() << std::endl;
    }
    if (argument.arg_interface.has_value()) {
        os << "interface: " << argument.arg_interface.value() << std::endl;
    }
    os << "}" << std::endl;
    return os;
}

std::ostream& operator<<(std::ostream& os, const WLAction& wlaction) {
    os << "name: " << wlaction.name << "\n";
    os << "description: " << wlaction.description << "\n";
    os << "arguments: [" << std::endl;
    for (const auto& argument : wlaction.arguments) {
        os << argument;
    }
    os << "]" << std::endl;
    return os;
}

std::ostream& operator<<(std::ostream& os, const WLRequest& wlreq) {
    os << "WLRequest: {\n";
    const WLAction& action_ref = wlreq;
    os << action_ref;
    os << "}" << std::endl;
    return os;
}

std::ostream& operator<<(std::ostream& os, const WLEvent& wlevent) {
    os << "WLEvent: {\n";
    const WLAction& action_ref = wlevent;
    os << action_ref;
    os << "}" << std::endl;
    return os;
}

std::ostream& operator<<(std::ostream& os, const WLEnum::WLEntry& wlenum) {
    os << "WLEntry: {\n";
    os << "name: " << wlenum.name << ",\n";
    if (wlenum.summary.has_value()) {
        os << "summary: " << wlenum.summary.value() << ",\n";
    }
    os << "value: " << wlenum.value << ",\n";
    os << "}" << std::endl;
    return os;
}

std::ostream& operator<<(std::ostream& os, const WLEnum& wlenum) {
    os << "WLEnum: {\n";
    os << "name: " << wlenum.name << ",\n";
    if (wlenum.description.has_value()) {
        os << wlenum.description.value() << ",\n";
    }
    os << "bitfield: " << std::boolalpha << wlenum.bitfield << ",\n";
    os << "entries: [" << std::endl;
    for (const auto& entry : wlenum.entries) {
        os << entry << std::endl;
    }
    os << "]\n" << std::endl;
    os << "}" << std::endl;
    return os;
}

std::ostream& operator<<(std::ostream& os, const WLInterface& wli) {
    os << "WLInterface: {\n";
    os << "name: " << wli.name << "\n";
    os << "description:\n" << wli.description << "\n";
    os << "requests: [" << std::endl;
    for (const auto& request : wli.requests) {
        os << request << "," << std::endl;
    }
    os << "]\n";
    os << "events: [" << std::endl;
    for (const auto& event : wli.events) {
        os << event << "," << std::endl;
    }
    os << "]\n";
    os << "enums: [" << std::endl;
    for (const auto& en : wli.enums) {
        os << en << "," << std::endl;
    }
    os << "]\n";
    os << "version: " << wli.version << "\n";
    os << "}" << std::endl;
    return os;
}

std::ostream& operator<<(std::ostream& os, const wg::WLDescription& wld) {
    os << "summary: " << wld.summary << "\ndescription: " << wld.description << std::endl;
    return os;
}
