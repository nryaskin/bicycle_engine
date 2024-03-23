#ifndef BE_MEMORY_WAYLAND_BC_WAYLAND_OUTPUT_H
#define BE_MEMORY_WAYLAND_BC_WAYLAND_OUTPUT_H
#include <type_traits>
#include <memory>
#include <vector>
#include <unordered_map>

#include "wayland/interface/bc_wayland_interface.hpp"
#include "wayland_client_header.h"
#include "noncopyable.hpp"

namespace bicycle_engine::wayland::interface {
    struct Geometry {
        int32_t x;
        int32_t y;
        int32_t physical_width;
        int32_t physical_height;
        int32_t subpixel;
        std::string make;
        std::string model;
        int32_t transform;
    };

    struct Mode {
        uint32_t flags;
        int32_t  width;
        int32_t  height;
        int32_t  refresh;
    };

    struct Scale {
        uint32_t scale;
    };

    struct Name {
        std::string name; 
    };

    struct Description {
        std::string description;
    };

    template<typename T>
    concept OutputAttributeType = std::is_same_v<T, Geometry>
                                || std::is_same_v<T, Mode>
                                || std::is_same_v<T, Scale>
                                || std::is_same_v<T, Name>
                                || std::is_same_v<T, Description>;

    class GenericCallbackBase {
    public:
        GenericCallbackBase() = default;
        ~GenericCallbackBase() {}
    };

    template <OutputAttributeType T>
    class GenericCallback : public GenericCallbackBase {
    public:
        using type = T;
        using ref_type = std::add_lvalue_reference_t<type>;
        using cref_type = std::add_const_t<ref_type>;

        GenericCallback() = default;
        void operator()(const int err, cref_type value);
    };

    using CallbacksVector = std::vector<std::shared_ptr<GenericCallbackBase>>;

    class WLOutputService: public WLInterface, noncopyable {
    public:
        using interface_native_t = struct wl_output;
        using interface_native_p = interface_native_t*;
        using listener_t = struct wl_output_listener;
        WLOutputService();
        WLOutputService(struct wl_registry * registry, uint32_t name, const char *interface, uint32_t version);
        // Movable
        WLOutputService(WLOutputService&&);
        WLOutputService& operator=(WLOutputService&&);

        // Let's think of this as blocking operation
        template<OutputAttributeType ReturnType>
        ReturnType& read();

        // This shall schedule read and then exit leaving hadnling to callback.
        template<OutputAttributeType T>
        void schedule_read(GenericCallback<T> cb);

        inline Mode& read_mode() { return read<Mode>(); }
#if 0
        Geometry& read_geometry();
        void schedule_read_geometry(GeometryCallback&& geo_cb);

        Mode& read_mode();
        void schedule_read_mode(ModeCallback&& mode_cb);

        uint32_t read_scale();
        void schedule_read_scale(ScaleCallback&& scale_cb);

        std::string& read_name();
        void schedule_read_name(NameCallback&& name_cb);

        std::string& read_description();
        void schedule_read_description(DescriptionCallback&& desc_cb);
#endif
#if 0
        const Geometry& geometry() const { return geometry_; }
        const Mode& mode() const { return mode_; }
        uint32_t scale() const { return scale_; }
        const std::string& name() const { return name_; }
        const std::string& description() const { return description_; }
#endif

        void add_geo_cb(const std::shared_ptr<GenericCallback<Geometry>>& cb) {
            add_cb(cb);
        }

        void add_mode_cb(const std::shared_ptr<GenericCallback<Mode>>& cb) {
            add_cb(cb);
        }

        void add_scale_cb(const std::shared_ptr<GenericCallback<Scale>>& cb) {
            add_cb(cb);
        }

        void add_name_cb(GenericCallback<Name> cb) {
            add_cb(cb);
        }

        void add_desc_cb(GenericCallback<Description> cb) {
            add_cb(cb);
        }

        template <OutputAttributeType T>
        void add_cb(const std::shared_ptr<GenericCallback<T>>& cb) {
            const auto& ti = typeid(T);
            auto callbacks = cb_map[ti];
            callbacks.push_back(std::static_pointer_cast<GenericCallbackBase>(cb));
        }

        interface_native_p get_native();
    private:
        static void geometry_handler(void *data,
                                     struct wl_output *wl_output,
                                     int32_t x,
                                     int32_t y,
                                     int32_t physical_width,
                                     int32_t physical_height,
                                     int32_t subpixel,
                                     const char *make,
                                     const char *model,
                                     int32_t transform);

        static void mode_handler(void *data,
                                 struct wl_output *wl_output,
                                 uint32_t flags,
                                 int32_t width,
                                 int32_t height,
                                 int32_t refresh);

        static void done_handler(void *data, struct wl_output *wl_output);

        static void scale_handler(void *data,
                                  struct wl_output *wl_output,
                                  int32_t factor);

        static void name_handler(void *data, struct wl_output *wl_output, const char *name);

        static void description_handler(void *data, struct wl_output *wl_output, const char *description);

        void swap(WLOutputService& lhs, WLOutputService& rhs);

        Geometry                            geometry_;
        Mode                                mode_;
        Scale                               scale_;
        Name                                name_;
        Description                         description_;
        std::unique_ptr<interface_native_t> wl_output_;

        // TODO: Maybe move to other entity(class)
        std::unique_ptr<WLOutputService::listener_t> wl_output_listener =
            std::make_unique<WLOutputService::listener_t>(&WLOutputService::geometry_handler,
                                                          &WLOutputService::mode_handler,
                                                          &WLOutputService::done_handler,
                                                          &WLOutputService::scale_handler,
                                                          &WLOutputService::name_handler,
                                                          &WLOutputService::description_handler);

        std::unordered_map<const std::type_info&, CallbacksVector> cb_map;
    };
}

#endif /* BE_MEMORY_WAYLAND_BC_WAYLAND_OUTPUT_H */
