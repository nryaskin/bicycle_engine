#ifndef CPP_INCL_H
#define CPP_INCL_H
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
        static constexpr char start = '<';
        static constexpr char end = '>';
    };

    class QuoteInclusion : public SourceFileInclusion {
    public:
        static constexpr char start = '"';
        static constexpr char end = '"';
    };

    class Includes : public Container<AngleInclusion, QuoteInclusion> {
    public:
        auto items() const {
            return container;
        }
    };
}

#endif /* CPP_INCL_H */
