#include <iostream>
#include <concepts>
#include "bc_wayland_client.hpp"
namespace bc = bicycle_engine;

//
// Ok so we are going to create window here.
// We are going to provide buffers to wayland compositor via frame listener.
// But we have our different set of rules which shall be driven by different clock.
// So we are going to set up service which is going to be updated by timer.
//
// Do we need to trigger redraw on each change?
// 
// We have screen with refresh rate of 120Hz, we shall provide updated frame at least each 120Hz
// Next thing we can consider that by doing updates after each world update we are spending a lot of resources.
// Ok so we shall update frames at least 120 times per second.
//

template<typename T>
concept is_graphic_device = std::derived_from<T, bc::GraphicsDevice<uint32_t>>;

// Sample class for drawing checker background
template<is_graphic_device gd_t>
class CheckerBoardBackground {
public:
    CheckerBoardBackground(
            size_t height_,
            size_t width_,
            size_t square_hight_,
            size_t square_width_,
            gd_t&  graphics_device_)
        : height(height_), width(width_), square_hight(square_hight_), square_width(square_width_), graphics_device(graphics_device_) {}

    void draw() {
        for (int y = 0; y < height; ++y) {
            for (int x = 0; x < width; ++x) {
                if ((x + y / 8 * 8) % 16 < 8)
                    graphics_device.DrawPixel(x, y, 0xFF666666);
                else
                    graphics_device.DrawPixel(x, y, 0xFFEEEEEE);
            }
        }
    }

    void change_offset(size_t offset_) {
        offset = offset_;
        draw();
    }

private:
    size_t height;
    size_t width;
    size_t square_hight;
    size_t square_width;
    size_t offset = 0;
    gd_t&  graphics_device;
};

//
// Ok, so I guess I need to block before enabling user with interaction with my grpahics_device
// First of all, what do we need to start working with wayland properly
//
// 1. We need some interfaces to be prepared for us which are prepared with roundtrip function. (Already done, don't need to do anything)
// 2. We need to query our environment on outputs and inputs and their capabilities which gives us depending on output ways to share buffers(TODO: Also I need to know what is going to happen in case of multiple output devices)
//    In case of multiple outputs there is listener to handle surface getting on ouput so I guess depending on that output we can render parts of surface differently.
//    I don't really get for now how wayland handles multiple outputs and one surface. can it provide different rendering depending on surface type or not?
// 3. Query compositor on color byte format.
//
// After all of this we can return from blocking and start execution.
//
//
// Ok, so we can do without quering output devices.
// I guess there can be two approaches.
// First, User can define required width and height of buffer.
// We are going to query for formats and when we receive enough format callbacks we are going to create buffer and now it shall be possible to start work with wayland surface.
// We are going to gather during execution information about joined new outputs and input if neccessary.
//
//

int main() {
    try {
        bc::WaylandClient wc;
        wc.set_title("My Wayland Client Experiment!");

#if 0
        while(true) {
            wc.dispatch();
        }
#endif
    }
    catch (...) {
        throw;
    }
    return 0;
}
