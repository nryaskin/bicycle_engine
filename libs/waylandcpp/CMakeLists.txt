add_library(waylandcpp-common INTERFACE)
target_include_directories(waylandcpp-common INTERFACE include)

add_subdirectory(apps/generator)
add_subdirectory(generated)

add_library(waylandcpp)
target_sources(waylandcpp
               PRIVATE
                src/socket.cpp
                src/object_builder.cpp
                src/buffer_parser.cpp
                src/buffer.cpp)
target_compile_features(waylandcpp PUBLIC cxx_std_20)
target_link_libraries(waylandcpp PUBLIC waylandcpp-common waylandcpp-generated)
