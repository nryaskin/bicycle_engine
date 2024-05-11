#ifndef WAYLANDCPP_WIRE_BUFFER_PARSER_H
#define WAYLANDCPP_WIRE_BUFFER_PARSER_H

#include "waylandcpp/wire/types.hpp"

namespace waylandcpp::wire {
    class WireBufferParser {
    public:
        struct Event {
            wire_object_id_t id;
            wire_op_t        op_code;
            WireBuffer       buffer;
        };

        WireBufferParser(WireBuffer& buffer);

        Event event();

        wire_object_id_t object_id();

        wire_uint_t uint();

        wire_string_t string();

        bool isEnd();

    private:
        WireBuffer& buffer_;
        uint32_t index = 0;
    };
}

#endif /* WAYLANDCPP_WIRE_BUFFER_PARSER_H */
