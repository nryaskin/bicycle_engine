find_package(Boost REQUIRED CONFIG COMPONENTS system)

add_executable(waylandcpp_generator)
target_sources(waylandcpp_generator PRIVATE main.cpp wlstream.cpp protocol.cpp)
target_compile_features(waylandcpp_generator PUBLIC cxx_std_20)
target_link_libraries(waylandcpp_generator PRIVATE Boost::system)
