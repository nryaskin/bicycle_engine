#ifndef  BICYCLE_ENGINE_H
#define BICYCLE_ENGINE_H

#include <cstdint>

namespace bicycle_engine {
    struct point_t {
        int x;
        int y;
    };

    template<typename T>
    class GraphicsDevice {
    public:
        virtual void clear(T color) = 0;
        void draw_pixel(point_t& p, T color) { draw_pixel(p.x, p.y, color); }
        virtual void draw_pixel(int x, int y, T color) = 0;
        virtual ~GraphicsDevice() {} 
    };
};

#endif /* BICYCLE_ENGINE_H */
