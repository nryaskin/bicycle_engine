#ifndef CPP_INCLUDES_H
#define CPP_INCLUDES_H
#include <string>
#include <variant>

#include "cpp/symbols.hpp"

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

        void sequential_all(auto&& action) const {
            action(directive);
            action(language::open_angle);
            action(file);
            action(language::close_angle);
        }
    };

    class QuoteInclusion : public SourceFileInclusion {
    public:
        QuoteInclusion() = default;
        QuoteInclusion(const std::string& name) { file = name; }
        void sequential_all(auto&& action) const {
            action(directive);
            action(language::double_quote);
            action(file);
            action(language::double_quote);
        }
    };

    using include_t = std::variant<AngleInclusion, QuoteInclusion>;
}

#endif /* CPP_INCLUDES_H */
