#ifndef CPP_INCLUDES_H
#define CPP_INCLUDES_H
#include <string>
#include <variant>

#include "container.hpp"

namespace cpp {
    // https://en.cppreference.com/w/cpp/preprocessor/include
    class SourceFileInclusion {
    public:
        static constexpr std::string directive = "#include";
        std::string file;
    };

    class AngleInclusion : public SourceFileInclusion {
    public:
        AngleInclusion() = default;
        AngleInclusion(const std::string& name) { file = name; }
        static constexpr std::string start = "<";
        static constexpr std::string end = ">";
    };

    class QuoteInclusion : public SourceFileInclusion {
    public:
        QuoteInclusion() = default;
        QuoteInclusion(const std::string& name) { file = name; }
        static constexpr std::string start = "\"";
        static constexpr std::string end = "\"";
    };

    class Includes : public Container<AngleInclusion, QuoteInclusion> {
    public:
        auto items() const {
            return container;
        }
    };
}

#endif /* CPP_INCLUDES_H */
