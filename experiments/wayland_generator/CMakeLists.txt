find_package(Boost REQUIRED CONFIG COMPONENTS system)

set(WAYLAND_GEN_SRCS
    main.cpp
    wlstream.cpp
    protocol.cpp
    cppbuilder.cpp)

add_executable(waylandcpp_generator)
target_sources(waylandcpp_generator PRIVATE ${WAYLAND_GEN_SRCS})
target_compile_features(waylandcpp_generator PUBLIC cxx_std_20)
target_link_libraries(waylandcpp_generator PRIVATE Boost::system cppgen)
