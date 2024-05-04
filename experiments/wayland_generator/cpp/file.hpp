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
#include "container.hpp"

namespace cpp {
    class File {
    public:
        File(const std::string& name);

        template<typename T>
        void add_include(T&& t) {
            includes.add(t);
        }

        std::string to_string() const;

    private:
        Includes includes;
        std::string name;
    };
 
    class Header :
        public File,
        public Container<Namespace, SimpleDeclaration, Function, Class> {

    public:
        Header(const std::string& name) : File(name + ".hpp") {}

        std::string to_string() const;

    private:
        // shall contain list of declarations.
        // Declarations
        //
        // It is list of variant of all of this, some are reqursive.
        // https://en.cppreference.com/w/cpp/language/declarations
        //
        // simple_decl { type, id, optional(initializer) }
        // wire_object_id_t id = 1;
        // wire_op_t sync_op = 0x0000;
        // wire_op_t get_registry_op = 0x0001;
        // 
        // namespace definition(Yes, this is declaration)
        // namesace { name, declarations...}
        //
        // function_definition(type, name, param_list)
        //
        // Class_declaration:
        // Methods
        //     void get_registry(wire_new_id_t registry_id); 
        //     void sync(wire_new_id_t callback_id);
        //     // Events
        //  void error(wire_object_id_t id, wire_uint_t code, std::string& msg);
        //     void delete_id(wire_object_id_t id);
        //
        //  Access_modifier(enum)
        //  public:
        //  private:
        //  protected:
    };

    class Source : public File {
        Source(const std::string& name) : File(name + ".cpp") {}
        // Definitions
    };
}


#endif /* CPP_FILE_H */
