#ifndef CPP_FILE_H
#define CPP_FILE_H 
#include <string>
#include <vector>
#include <variant>

#include "incl.hpp"
#include "namespace.hpp"
#include "simple_decl.hpp"
#include "function.hpp"
#include "access_mod.hpp"
#include "definition.hpp"
#include "container.hpp"

namespace cpp {
    class Document;

    class File {
    public:
        File(const std::string& name);

        template<typename T>
        void add_include(T&& t) {
            includes.add(std::forward<T>(t));
        }

        std::string get_name() const { return name; }

        friend Document& operator<<(Document&, const File&);
    private:
        Includes includes;
        std::string name;
    };
 
    class Header :
        public File,
        public Container<Namespace, SimpleDeclaration, Function, Class> {

    public:
        Header(const std::string& name) : File(name + ".hpp") {}

        friend Document& operator<<(Document&, const Header&);
    };

    class Source : public File {
    public:
        Source(const std::string& name) : File(name + ".cpp") {}
        void set_namespace(Namespace ns) { ns_ = ns; }
        void add(Definition def);

        friend Document& operator<<(Document&, const Source&);
    private:
        Namespace ns_;
        std::vector<Definition> definitions;
    };
}


#endif /* CPP_FILE_H */
