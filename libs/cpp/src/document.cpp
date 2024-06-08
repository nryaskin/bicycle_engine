#include "cpp/declaration/document.hpp"

#include <cassert>
#include <fstream>
#include <format>

namespace fs = std::filesystem;

namespace cpp {
    std::string Document::Line::get() const {
        std::string line(ident, ' '); 

        line.append(text);

        return line;
    }

    Document::Document(const std::string& name, int max_width, int ident) : name(name), max_width(max_width), ident_unit(ident) {}

    void Document::append_token(const token_t& token) {
        current_tokens.push_back(token);
    }

    void Document::append_tokens(const std::vector<token_t>& tokens) {
        current_tokens.insert(current_tokens.end(), tokens.begin(), tokens.end());
    }

    void Document::ident_incr() { current_ident += ident_unit; }

    void Document::ident_decr() {
        current_ident -= ident_unit;
        if (current_ident < 0) {
            throw std::runtime_error("Ident cannot be less then zero!");
        }
    }

    void Document::finish_block() {
        std::string current_line;

        for (auto& token : current_tokens) {
            if (current_line.size() + token.size() > max_width) {
                // Warning: This is naive approach since for some string at least it can break them.
                lines.push_back(Line(current_line, current_ident));
                current_line.clear();
            }
            current_line.append(token);
        }

        lines.push_back(Line(current_line, current_ident));
        current_tokens.clear();
    }

    void Document::save(const fs::path& path) {
        if (!fs::is_directory(path)) {
            throw std::runtime_error(std::format("Path shall be directory: {}", path.string()));
        }

        std::string full_path = path / name;
        std::fstream f(full_path, f.out | f.trunc);
        for (const auto& line : lines) {
            f << line.get() << std::endl;
        }
    }
}
