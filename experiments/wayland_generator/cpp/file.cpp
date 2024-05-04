#include <format>

#include "file.hpp"

namespace cpp {
    File::File(const std::string& name) : name(name){}

    std::string File::to_string() const {
        std::string file;

        for (auto& include: includes.items()) {
            std::visit([&file](auto&& object) {
                using T = std::decay_t<decltype(object)>;
                if constexpr (!std::is_same_v<T, std::monostate>) {
                file.append(std::format("{} {}{}{}\n", 
                            object.directive,
                            object.start,
                            object.file,
                            object.end));
                }
            }, include);
        }

        return file;
    }

    std::string Header::to_string() const {
        std::string header = File::to_string();

        for (const auto& decl : container) {
            std::visit([&header](auto&& object) {
                using T = std::decay_t<decltype(object)>;
                if constexpr (!std::is_same_v<T, std::monostate>) {
                    header.append(object.to_string());
                    header.append("\n");
                }
            }, decl);
        }

        return header;
    }
}
