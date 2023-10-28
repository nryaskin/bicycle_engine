#include <iostream>
#include "bc_wayland_client.hpp"
namespace bc = bicycle_engine;

int main() {
    bc::WaylandClient wc;
    wc.SetTitle("My Wayland Client Experiment!");
    while(true) {
        wc.Dispatch();
    }
    return 0;
}
