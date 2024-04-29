#ifndef WAYLAND_GENERATOR_WLSTREAM_H
#define WAYLAND_GENERATOR_WLSTREAM_H

#include "wire_types.hpp"
#include "protocol.hpp"
#include <iostream>

namespace wg = wayland::generator;

std::ostream& operator<<(std::ostream& os, const wg::wire_type& type);
std::ostream& operator<<(std::ostream& os, const wg::WLArgument& argument);
std::ostream& operator<<(std::ostream& os, const wg::WLAction& a);
std::ostream& operator<<(std::ostream& os, const wg::WLRequest& req);
std::ostream& operator<<(std::ostream& os, const wg::WLEvent& req);
std::ostream& operator<<(std::ostream& os, const wg::WLEnum& req);
std::ostream& operator<<(std::ostream& os, const wg::WLInterface& wli);
std::ostream& operator<<(std::ostream& os, const wg::WLDescription& wli);


#endif /* WAYLAND_GENERATOR_WLSTREAM_H */
