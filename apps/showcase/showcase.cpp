#include <iostream>
#include <concepts>
#include <cassert>
#include "bc_wayland_client.hpp"
#include "graphic_device.hpp"
namespace bew = bicycle_engine::wayland;

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

#if 0
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
#endif
// WaylandClient responsibility:
// 1. query settings
// 2. cache settings
// 3. allocate/deallocate wayland resources

// WaylandSurface responsibility:
// 1. handle interaction events
// 2. output buffer

int main() {
    try {
        bew::WaylandClient wc;
        auto surface = wc.create_toplevel_surface();

#if 0
        // TODO: Need to wait for output for gather information first.
        auto output = wc.create_output();
        wc.async_create_output(cbs);
        using mode_t = bicycle_engine::wayland::interface::Mode;
        // We either blocking here. Here we actually can block and don't care.
        // We cannot block here. I mean we can block, we just 
        auto mode = output.read_mode();
        // Or we scheduling callback.
        //TODO: Check how to use lambda and functions interchanably
        output.schedule_read([](const int err, const mode_t& mode){


                });
        // Or
        output.schedule_read(receiver_func);

        // Instead of
        int32_t width = output.mode().width;
        int32_t height = output.mode().height;

        assert(wc.get_shm().available_formats().find(WL_SHM_FORMAT_ARGB8888) != wc.get_shm().available_formats().end());
        wl_shm_format format = WL_SHM_FORMAT_ARGB8888;

        int32_t stride = 4 * mode.width;
        constexpr uint32_t buffers_count = 2;
        bc::memory::SharedMemory shared_mem(buffers_count * stride * mode.height);
        // TODO: Think do I need some async here or not, can it block?
        auto sm_pool = wc.create_shared_memory_pool(shared_mem);
        auto buffer = sm_pool.create_buffer(0, mode.width, mode.height, stride, format);

        // surface.attach(buffer);
        // surface.set_buffer_transform();
        // surface.set_buffer_scale();
#endif
/**
 *  // BUFFER
 *     WLBuffer buffer = shm_pool.create_buffer(offset, width, height, stride, format);
 *
 *      for (int x = 0; x < surface.height(); x++) {
 *          for (int y = 0; y < surface.width(); y++) {
 *              buffer::format&& color = 0xFFFF00;
 *              buffer[x, y] = color;
 *          }
 *      }
 *  // BUFFER END
 *      surface.damage_buffer(x. y. surface.width(), surface.height());
 *  // Apply surface state
 *      surface.commit();
 *  // At this point we shall have surface on screen of screen size which is colored as 0xFFFF00
*/

#if 1
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
