set(BE_INCLUDE_DIRS ${BicycleEngine_SOURCE_DIR}/include)
set(BE_SRCS bc-statistics.cpp
            bc-shm.cpp
            bc-shm-file.cpp
            wayland/output.cpp
            wayland/display.cpp
            wayland/surface.cpp
            wayland/registry.cpp
            bc-wayland-frame-callback.cpp
            ui_context.cpp)

add_library(bicycle_engine STATIC)
target_include_directories(bicycle_engine PUBLIC ${BE_INCLUDE_DIRS})
target_sources(bicycle_engine PRIVATE ${BE_SRCS})
target_compile_features(bicycle_engine PUBLIC cxx_std_20)

find_package(Boost REQUIRED CONFIG COMPONENTS system)

target_link_libraries(bicycle_engine PUBLIC wayland-api PRIVATE rt xkbcommon Boost::system)
