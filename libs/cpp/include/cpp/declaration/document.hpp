#ifndef CPP_DOCUMENT_H
#define CPP_DOCUMENT_H

#include <vector>
#include <string>
#include <filesystem>

namespace cpp {
    class Document {
        class Line {
        public:
            Line(const std::string& text = "", int ident = 0) : ident(ident), text(text) {}
            int size() const { return text.size() + ident; }

            std::string get() const;
        private:
            int         ident;
            std::string text;
        };

    public:
        using token_t = std::string;
        Document(const std::string& name, int max_width, int ident = 2);

        void append_token(const token_t& token);
        void append_tokens(const std::vector<token_t>& tokens);

        void ident_incr();
        void ident_decr();
        void finish_block();
        void save(const std::filesystem::path& path);
    private:
        std::string          name;
        int                  max_width;
        const int            ident_unit;
        int                  current_ident = 0;
        std::vector<token_t> current_tokens;
        std::vector<Line>    lines;
    };
}

#endif /* CPP_DOCUMENT_H */
