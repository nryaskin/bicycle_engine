#ifndef WAYLANDCPP_WIRE_TYPES_H
#define WAYLANDCPP_WIRE_TYPES_H

#include <concepts>
#include <cstdint>
#include <cstring>
#include <string>

#include "waylandcpp/wire/buffer.hpp"

template<typename T>
concept WireType = requires (T t, waylandcpp::wire::WireBuffer& wb, int start) {
    { t.size() } -> std::same_as<std::size_t>;
    { t.put(wb, start) } -> std::same_as<void>;
};

namespace waylandcpp::wire {
    using wire_op_t = uint16_t;
    using wire_size_t = uint16_t;
    using wire_word_t = uint32_t;

    class wire_int_t {
    public:
        wire_int_t(int32_t v) : value(v) {}

        std::size_t size() const {
            return sizeof(decltype(value)) / sizeof(uint32_t);
        }

        void put(WireBuffer& wb, int start) {
            auto& word = wb[start];
            word = value;
        }

        int32_t value;
    };

    class wire_uint_t {
    public:
        std::size_t size() const {
            return sizeof(decltype(value)) / sizeof(uint32_t);
        }

        void put(WireBuffer& wb, int start) {
            std::memcpy(&wb[start], &value, size());
        }

        wire_uint_t(uint32_t val) : value(val) {}
        uint32_t value;
    };

    class wire_fixed_t {
    public:
        std::size_t size() const {
            return sizeof(decltype(value)) / sizeof(uint32_t);
        }

        void put(WireBuffer& wb, int start) {
            std::memcpy(&wb[start], &value, size());
        }
        uint32_t value;
    };

    // TODO: This passed as ancillary data so I need to figure out how to get it from unix domain socket.
    class wire_fd_t {
    public:
        std::size_t size() const { return 0; }
        void put(WireBuffer& wb, int start) {}
    };


    class wire_new_id_t {
    public:
        std::size_t size() const { return sizeof(decltype(value)) / sizeof(uint32_t); }

        void put(WireBuffer& wb, int start) {
            std::memcpy(&wb[start], &value, size());
        }

        wire_new_id_t(uint32_t val) : value(val) {}
        uint32_t value;
    };

    class wire_object_id_t {
    public:
        std::size_t size() const { return sizeof(decltype(value)) / sizeof(uint32_t); }

        void put(WireBuffer& wb, int start) {
            std::memcpy(&wb[start], &value, size());
        }

        wire_object_id_t(uint32_t val = 0) : value(val) {}
        uint32_t value;

        auto operator<=>(const wire_object_id_t&) const = default;
    };

    class wire_string_t {
    public:
        // First byte is 32-bit length of string include null-terminating symbol.
        // Size as if it were uint32_t array.
        std::size_t size() const {
            // 32-bit length takes always first 32 bits.
            std::size_t size = 1;

            if (value != "") {
                auto bytes_with_nt = value.size() + 1;
                size += (bytes_with_nt) / sizeof(uint32_t);
                size += (bytes_with_nt % sizeof(uint32_t) != 0) ? 1: 0;
            }

            return size;
        }

        void put(WireBuffer& wb, int start) {
            int sidx = 0;
            int end = start + size();
            wb[start++] = (value.size() == 0) ? 0 : value.size() + 1;
            while(start != end - 1) {
                auto& word = wb[start++];
                word = (value[sidx] | value[sidx + 1] << 8 | value[sidx + 2] << 16 | value[sidx + 3] << 24);
                sidx += sizeof(uint32_t);
            }

            auto& word = wb[start];
            word = 0;
            while (sidx < value.size()) {
                auto mod = (sidx % sizeof(uint32_t));
                word |= value[sidx] << (mod * 8);
            }
        }

        wire_string_t(std::string& str) : value(str) {}

        std::string value;
    };

    class wire_array_t {
        std::size_t size() const { return 0; }
        void put(WireBuffer& wb, int start) {}
    };

}

#endif /* WAYLANDCPP_WIRE_TYPES_H */
