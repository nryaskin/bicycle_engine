#ifndef BE_OUTPUT_H
#define BE_OUTPUT_H
#include <cstdint>
#include <string>
#include <format>

#include "wayland/output.hpp"

namespace bicycle_engine {
    class OutputDescription {
    public:
        friend std::ostream& operator<<(std::ostream& os, const OutputDescription& od);

        std::string subpixel_description() const {
            switch (subpixel) {
                case WL_OUTPUT_SUBPIXEL_UNKNOWN:
                    return "Unknown";
                case WL_OUTPUT_SUBPIXEL_NONE:
                    return "None";
                case WL_OUTPUT_SUBPIXEL_HORIZONTAL_RGB:
                    return "Horizontal RGB";
                case WL_OUTPUT_SUBPIXEL_HORIZONTAL_BGR:
                    return "Horizontal BGR";
                case WL_OUTPUT_SUBPIXEL_VERTICAL_RGB:
                    return "Vertical RGB";
                case WL_OUTPUT_SUBPIXEL_VERTICAL_BGR:
                    return "Vertical BGR";
                default:
                    return "Unhandled format";
            }
        }

        std::string transform_description() const {
            switch (transform) {
                case WL_OUTPUT_TRANSFORM_NORMAL:
                    return "no transform";
                case WL_OUTPUT_TRANSFORM_90:
                    return "1-90 degrees counter-clockwise";
                case WL_OUTPUT_TRANSFORM_180:
                    return "2-180 degrees counter-clockwise";
                case WL_OUTPUT_TRANSFORM_270:
                    return "3-270 degrees counter-clockwise";
                case WL_OUTPUT_TRANSFORM_FLIPPED:
                    return "4 - 180 degree flip around a vertical axis";
                case WL_OUTPUT_TRANSFORM_FLIPPED_90:
                    return "5 - flip and rotate 90 degrees counter-clockwise";
                case WL_OUTPUT_TRANSFORM_FLIPPED_180:
                    return "6 - flip and rotate 180 degrees counter-clockwise";
                case WL_OUTPUT_TRANSFORM_FLIPPED_270:
                    return "7 - flip and rotate 270 degrees counter-clockwise";
                default:
                    return "Unhandled transform";
            }
        }

        std::string mode_description() const {
            switch (flags) {
                case WL_OUTPUT_MODE_CURRENT:
                    return "current";
                case WL_OUTPUT_MODE_PREFERRED:
                    return "prefferd";
                default:
                    return "Unhandled mode";
            }
        }

        void geometry_cb(int32_t x,
                         int32_t y,
                         int32_t physical_width,
                         int32_t physical_height,
                         int32_t subpixel,
                         const std::string& make,
                         const std::string& model,
                         int32_t transform) {
            this->x = x;
            this->y = y;
            this->physical_width = physical_width;
            this->physical_height = physical_height;
            this->subpixel = subpixel;
            this->make = make;
            this->model = model;
            this->transform = transform;
        }

        void mode_cb(uint32_t flags,
                     int32_t  width,
                     int32_t  height,
                     int32_t  refresh) {
            this->flags = flags;
            this->width = width;
            this->height = height;
            this->refresh = refresh;
        }

        void scale_cb(int32_t factor) {
            this->factor = factor;
        }

        void name_cb(const std::string& name) {
            this->name = name;
        }

        void description_cb(const std::string& description) {
            this->description = description;
        }

    private:
        int32_t     x;
        int32_t     y;
        int32_t     physical_width;
        int32_t     physical_height;
        int32_t     subpixel;
        std::string make;
        std::string model;
        int32_t     transform;
        uint32_t    flags;
        int32_t     width;
        int32_t     height;
        int32_t     refresh;
        int32_t     factor;
        std::string name;
        std::string description;
    };

    inline std::ostream& operator<<(std::ostream& os, const OutputDescription& od) {
        os << std::format("Manufacturer : {}\n", od.make);
        os << std::format("Model: {}\n", od.model);
        os << std::format("Name: {}\n", od.name);
        os << std::format("Description: {}\n", od.description);
        os << std::format("Position within the global compositor space: [{},{}]\n", od.x, od.y);
        os << std::format("Physical size: {} mm x {} mm\n", od.physical_width, od.physical_height);
        os << std::format("Subpixel: {}\n", od.subpixel_description());
        os << std::format("Transform: {}\n", od.transform_description());
        os << std::format("Resolution: {} x {}, refresh rate: {} mHZ, mode: {}\n", od.width, od.height, od.refresh, od.mode_description());
        os << std::format("Scale: {}\n", od.factor);
        return os;
    }
}
#endif /* BE_OUTPUT_H */
