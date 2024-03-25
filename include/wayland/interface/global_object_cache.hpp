#ifndef BE_WAYLAND_GLOBAL_OBJECT_CACHE_HPP
#define BE_WAYLAND_GLOBAL_OBJECT_CACHE_HPP
#include <unordered_map>
#include <string>
#include <cstdint>
#include "noncopyable.hpp"

namespace bicycle_engine::wayland::interface {
    class GlobalObjectCache : noncopyable {
        public:
            using numeric_name_t = uint32_t;
            using version_t = uint32_t;
            struct GlobalObjectData {
                numeric_name_t  name;
                std::string     interface;
                version_t       version;
            };
            
            GlobalObjectCache() {}
            GlobalObjectCache(GlobalObjectCache&& other) {
                *this = std::move(other);
            }

            GlobalObjectCache& operator=(GlobalObjectCache&& other) {
                std::swap(num_to_interface, other.num_to_interface);
                std::swap(name_to_interface, other.name_to_interface);
                return *this;
            }

            void add(std::string interface, numeric_name_t name, version_t version) {
                // TODO: Think about synchronization. For now I think there is no need since we are going to call wayland loop at least for this objects from one thread.
                GlobalObjectData go = {
                    .name = name,
                    .interface = interface,
                    .version = version
                };
                num_to_interface[name] = go;
                name_to_interface[interface] = go;
            }

            GlobalObjectData get(const std::string& interface) const {
                auto go = name_to_interface.find(interface);
                return go->second;
            }

            void remove(numeric_name_t name) {
                if (auto itr = num_to_interface.find(name); itr != num_to_interface.end()) {
                    auto& interface = itr->second.interface;
                    remove(name, interface);
                }
            }

            void remove(std::string& interface) {
                if (auto itr = name_to_interface.find(interface); itr != name_to_interface.end()) {
                    auto& name = itr->second.name;
                    remove(name, interface);
                }
            }

        private:
            void remove(numeric_name_t name, std::string& interface) {
                num_to_interface.erase(name);
                name_to_interface.erase(interface);
            }
            std::unordered_map<numeric_name_t, GlobalObjectData> num_to_interface;
            std::unordered_map<std::string, GlobalObjectData> name_to_interface;
    };
}

#endif /* BE_WAYLAND_GLOBAL_OBJECT_CACHE_HPP */
