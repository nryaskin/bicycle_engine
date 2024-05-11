find_package(Boost REQUIRED CONFIG COMPONENTS system)

add_executable(waylandcpp-generator)
target_sources(waylandcpp-generator
               PRIVATE
                main.cpp
                wlstream.cpp
                protocol.cpp
                cppbuilder.cpp)
target_compile_features(waylandcpp-generator PUBLIC cxx_std_20)
target_link_libraries(waylandcpp-generator PRIVATE Boost::system cppgen)
