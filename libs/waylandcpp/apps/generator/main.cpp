#include <string>
#include <iostream>
#include <sstream>
#include <format>
#include <filesystem>

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>

#include "protocol.hpp"
#include "wlstream.hpp"
#include "cppbuilder.hpp"

// CPP
#include "cpp/declaration/class.hpp"
#include "cpp/transform/text.hpp"

namespace fs = std::filesystem;
namespace pt = boost::property_tree;
namespace wg = wayland::generator;

class GeneratorArgs {
public:
    /**
     * Parse input strings.
     *
     * Parse input of the form "--config" "<config>" "--output" "<output>" --dry-run
     * In case of other options throws runtime_error("Unknown option: <option_name>")
     */
    GeneratorArgs(int argc, char *argv[]) {
        for (int i = 1; i < argc; ++i) {
            std::string arg(argv[i]);
            if (!arg.starts_with("--")) {
                throw std::runtime_error(std::format("Argument '{}' is not an option or option value!", arg));
            }


            arg.erase(0, 2);

            if (arg == "dry-run") {
                dry_run = true;
                continue;
            }

            if (i + 1 >= argc) {
                throw std::runtime_error(std::format("Cannot find value for option: '{}'", arg));
            }
            if (arg == "config") {
                config = argv[++i]; 
            } else if (arg == "output") {
                output = argv[++i];
            } else {
                throw std::runtime_error(std::format("Option '{}' is not supported!", arg));
            }
        }
    }

    fs::path config = "/usr/share/wayland/wayland.xml";
    fs::path output = "tmp";
    bool dry_run = false;
};

int main(int argc, char *argv[]) {
    GeneratorArgs args(argc, argv);

    std::vector<wg::WLInterface> interfaces;
    pt::ptree tree;

    std::cout << std::format("Parsing wayland protocol spec: '{}', generated source dir: '{}'", args.config.string(), args.output.string()) << std::endl;
    pt::read_xml(args.config, tree);
    auto xml_interfaces = tree.get_child("protocol");

    // Q: Why this form sometimes doesn't work?
    auto protocol_name = tree.get_child("protocol.<xmlattr>.name").data();
    std::cout << "Protocol name: " << protocol_name << std::endl;
    for (auto& interface: xml_interfaces) {
        if (interface.first == "interface") {
            interfaces.push_back(wg::WLInterface::create_from_xml(interface.second));
        }
    }

#if 0
// TODO: Add verbose option to generator
    for (auto& interface: interfaces) {
        std::cout << interface << std::endl;
    }
#endif

    if (args.dry_run) {
        std::fstream f(args.output.string() + "/files.cmake", f.out);
        if (!f.is_open()) {
            throw std::runtime_error(std::format("Cannot open file)"));
        }
        auto header_dir = args.output / "include" / "waylandcpp";
        auto source_dir = args.output / "src";

        f << "set(GENERATED_SOURCES ";
        for (auto& interface : interfaces) {
            f << std::format("{}.cpp\n", std::string("src/") + interface.name);
        }
        f << ")\n\n";

        f << "set(GENERATED_HEADERS ";
        for (auto& interface : interfaces) {
            f << std::format("{}.hpp\n", std::string("include/waylandcpp/") + interface.name);
        }
        f << ")\n";
    } else {
        std::vector<cpp::include_t> includes;
        includes.push_back(cpp::AngleInclusion("string"));
        includes.push_back(cpp::AngleInclusion("vector"));
        includes.push_back(cpp::QuoteInclusion("waylandcpp/wire/types.hpp"));
        includes.push_back(cpp::QuoteInclusion("waylandcpp/wire/socket.hpp"));
        includes.push_back(cpp::QuoteInclusion("waylandcpp/wire/object_builder.hpp"));

        wayland::generator::Builder builder(includes);

        if(!std::filesystem::exists(args.output)) {
            throw std::runtime_error(std::format("Output dir: '{}' doesn't exist!", args.output.string()));
        }

        auto header_dir = args.output / "include" / "waylandcpp";
        std::filesystem::create_directory(header_dir.parent_path());
        std::filesystem::create_directory(header_dir);

        auto source_dir = args.output / "src";
        std::filesystem::create_directory(source_dir);

        for (const auto& interface : interfaces) {
            auto text = builder.build(interface);

            auto header_file = header_dir / interface.name;
            std::cout << "HEADER: " << header_file << std::endl;
            std::cout << text << std::endl;
            // text.to_file(header_file);
        }

    }

    return 0;
}
