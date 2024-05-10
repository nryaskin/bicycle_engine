#ifndef CPP_FORMATTER_H
#define CPP_FORMATTER_H

#include "document.hpp"
#include "file.hpp"
#include "type.hpp"
#include "function.hpp"
#include <cassert>

namespace cpp {

    // Remove it since type by itself is not going to get into line.
    inline Document& operator<<(Document& doc, const Type& type) {
        doc.append_token(type.id());
        return doc;
    }

    inline Document& operator<<(Document& doc, const Ref& type) {
        doc.append_token(std::format("{}&", type.type.id()));
        return doc;
    }

    inline Document& operator<<(Document& doc, const RRef& type) {
        doc.append_token(std::format("{}&&", type.type.id()));
        return doc;
    }

    //Why even consider only lines?

    inline Document& operator<<(Document& doc,const TypeVariant& type) {
        std::visit([&doc] (auto&& object) {
                using T = std::decay_t<decltype(object)>;
                if constexpr (std::same_as<T, std::monostate>) {
                    assert(false);
                } else {
                    doc << object;
                }
        }, type.type);
        return doc;
    }

    inline Document& operator<<(Document& doc, const File& file) {
        for (auto& include: file.includes.items()) {
            std::visit([&doc](auto&& object) {
                using T = std::decay_t<decltype(object)>;
                if constexpr (!std::is_same_v<T, std::monostate>) {
                    doc.append_tokens({ object.directive, " ", object.start, object.file, object.end });
                    // TODO: Think of other ways to use it. For now I am forcing something like block end when I desire but not sure.
                    doc.finish_block();
                }
            }, include);
        }
        return doc;
    }

    inline Document& operator<<(Document& doc, const Header& header) {
        const File& file = header;
        doc << file;

        for (const auto& decl : header.container) {
            std::visit([&doc](auto&& object) {
                using T = std::decay_t<decltype(object)>;
                if constexpr (!std::is_same_v<T, std::monostate>) {
                    doc << object;
                }
            }, decl);
        }

        return doc;
    }

    inline Document& operator<<(Document& doc, const Source& source) {
        for (const auto& def : source.definitions) {
            doc << def;
        }

        return doc;
    }

    inline Document& operator<<(Document& doc, const Namespace& ns) {
        doc.append_tokens({ Namespace::keyword, " ", ns.id_, " ", "{" });
        doc.finish_block();
        doc.ident_incr();

        for (const auto& decl: ns.container) {
            std::visit([&doc](auto&& object) {
                using T = std::decay_t<decltype(object)>;
                if constexpr (!std::same_as<T, std::monostate>) { 
                    doc << object;
                }
            }, decl);
        }

        doc.ident_decr();
        doc.append_token("}");
        doc.finish_block();
        return doc;
    }

    inline Document& operator<<(Document& doc, const SimpleDeclaration& sd) {
        if (sd.initializer_.size() == 0) {
            doc << sd.type_;
            doc.append_tokens({" ", sd.id_, ";"});
        }
        else {
            doc << sd.type_;
            doc.append_tokens({" ", sd.id_, " ", sd.initializer_, ";"});
        }
        doc.finish_block();

        return doc;
    }

    inline Document& operator<<(Document& doc, const Function& func) {
        bool comma = false;

        doc << func.return_type;
        doc.append_tokens({ " ", func.name(), "(" });

        for (const auto& parameter: func.parameters) {
            if (comma) {
                doc.append_tokens({",", " "});
            }
            doc << parameter;
            comma = true;
        }
        doc.append_tokens({")", ";"});
        doc.finish_block();

        return doc;
    }

    inline Document& operator<<(Document& doc, const Parameter& param) {
        if (param.default_init.size() == 0) {
            doc << param.type;
            doc.append_tokens({ " ", param.name()});
        }
        else {
            doc << param.type;
            doc.append_tokens({ " ", param.name(), " ", param.default_init });
        }
        return doc;
    }

    inline Document& operator<<(Document& doc, const Class& cs) {

        std::stringstream ss;

        switch(cs.class_key) {
            case ClassKey::CLASS:
                doc.append_token("class");
                break;
            case ClassKey::STRUCT:
                doc.append_token("struct");
                break;
            case ClassKey::UNION:
                doc.append_token("union");
                break;
        }

        doc.append_tokens({" ", cs.class_head_name, " "});
        if (cs.base_clause.size() > 0) {
            doc.append_tokens({":", " "});
            bool comma = false;
            for (const auto& base : cs.base_clause) {
                if (comma) {
                    doc.append_tokens({",", " "});
                }
                doc.append_tokens({ inheritance_to_string(base.first), " ", base.second.class_head_name});
                comma = true;
            }
        }
        doc.append_tokens({" ", "{"});
        doc.finish_block();
        doc.ident_incr();

        for (const auto& ctr : cs.constructors) {
            doc.append_token(cs.class_head_name);
            doc << ctr;
        }
        doc.finish_block();

        for (const auto& decl: cs.container) {
            std::visit([&doc](auto&& object) {
                using T = std::decay_t<decltype(object)>;
                if constexpr (!std::same_as<T, std::monostate>) { 
                    doc << object;
                }
            }, decl);
        }
        doc.ident_decr();
        doc.append_tokens({"}" , ";"});
        doc.finish_block();

        return doc;
    }

    inline Document& operator<<(Document& doc, const Class::Ctr& ctr) {
        bool comma = false;
        doc.append_token("(");

        for (const auto& param: ctr.parameters) {
            if (comma) {
                doc.append_tokens({",", " "});
            }
            doc << param;
            comma = true;
        }

        doc.append_tokens({")", ";"});
        doc.finish_block();
        return doc;
    }

    inline Document& operator<<(Document& doc, const AccessModifier& am) {
        // NOTE: It's a hack because I like when access modifier aligns with class keyword.
        doc.ident_decr();
        switch(am.mod) {
            case access_modifier_t::PRIVATE:
                doc.append_tokens({"private", ":"});
                break;
            case access_modifier_t::PUBLIC:
                doc.append_tokens({"public", ":"});
                break;
            case access_modifier_t::PROTECTED:
                doc.append_tokens({"protected", ":"});
                break;
        }
        doc.finish_block();
        doc.ident_incr();
        return doc;
    }

    inline Document& operator<<(Document& doc, const Enum& e) {
        doc.append_tokens({"enum", " ", "class", " ", e.name, " ", ":", " "});
        doc << e.base;
        doc.append_token("{");
        doc.finish_block();
        doc.ident_incr();
        for (const auto& item : e.entities) {
            doc << item;
        }
        doc.ident_decr();
        doc.append_tokens({"}", ";"});
        doc.finish_block();
        return doc;
    }

    inline Document& operator<<(Document& doc, const Enum::Entity& entity) {
        doc.append_tokens({ entity.name, " ", "=", " ", entity.value, "," });
        doc.finish_block();
        return doc;
    }

    inline Document& operator<<(Document& doc, const Definition& def) {
        bool comma = false;
        if (def.method_.has_value()) {

            const auto& method = def.method_.value();
            doc << method.return_type;
            doc.append_tokens({ " ", def.class_.class_head_name, "::", method.name(), "(" });
            for (const auto& parameter : def.method_.value().parameters) {
                if (comma) {
                    doc.append_tokens({",", " "});
                }
                doc << parameter;
                comma = true;
            }

        } else {
            doc.append_tokens({ def.class_.class_head_name, "::", def.class_.class_head_name, "(" });
            for (const auto& parameter : def.method_.value().parameters) {
                if (comma) {
                    doc.append_tokens({",", " "});
                }
                doc << parameter;
                comma = true;
            }
        }
        doc.append_tokens({")", " ", "{"});
        doc.finish_block();
        doc.ident_incr();
        doc << def.method_body_;
        doc.ident_decr();
        doc.append_token("}");
        doc.finish_block();

        return doc;
    }

    inline Document& operator<<(Document& doc, const MethodBody& mb) {
        for(const auto& line : mb.lines) {
            doc.append_token(line);
            doc.finish_block();
        }
        return doc;
    }
}

#endif /* CPP_FORMATTER_H */
