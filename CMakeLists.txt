cmake_minimum_required(VERSION 3.25)

project(TestGraphics
        VERSION 0.1
        LANGUAGES CXX C)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(BC_PRIVATE_INC_DIRS include)
set(BC_SRCS src/bicycle-engine.cpp
            src/bc-wayland-client.cpp
            src/bc-wayland-shm.cpp
            src/bc-statistics.cpp
            src/xdg-shell-protocol.c)
add_executable(bicycle_engine ${BC_SRCS})
target_include_directories(bicycle_engine PRIVATE ${BC_PRIVATE_INC_DIRS})
set_target_properties(bicycle_engine PROPERTIES CXX_EXTENSIONS OFF)
# Build wayland client's header and library

#cc -o wl-client-sample wl-client-sample.c  xdg-shell-protocol.c -lwayland-client -lrt -lxkbcommon -g
target_link_libraries(bicycle_engine wayland-client rt xkbcommon)
