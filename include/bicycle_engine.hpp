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
        virtual void Clear(T color) = 0;
        void DrawPixel(point_t& p, T color) { DrawPixel(p.x, p.y, color); }
        virtual void DrawPixel(int x, int y, T color) = 0;
        virtual ~GraphicsDevice() {} 
    };
};

#endif /* BICYCLE_ENGINE_H */
