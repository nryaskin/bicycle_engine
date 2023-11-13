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

int main() {
    bc::WaylandClient wc;
    wc.SetTitle("My Wayland Client Experiment!");
    wc.Clear(0x0);

    //Create cbb and update value in different thread.
    CheckerBoardBackground cbb(1, 1, 8, 8, wc);
    cbb.change_offset(0);

    while(true) {
        wc.Dispatch();
    }
    return 0;
}
