#include "ui_context.hpp"

#include <iostream>

#include <boost/asio.hpp>


int main() {
    boost::asio::io_context io;

    bicycle_engine::UIContext ui(io);
    ui.read_outputs([&io] (auto output) { std::cout << output; });
    io.run();
    return 0;
};
