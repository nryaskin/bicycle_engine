cmake_minimum_required(VERSION 3.25..3.27)

project(BicycleEngine
        VERSION 0.1
        DESCRIPTION "Different graphics api's playground"
        LANGUAGES CXX C)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(cmake/checks.cmake)
#include(cmake/wayland_generator.cmake)
check_not_source()

add_subdirectory(libs)
## C Wayland API
#set(WAYLAND_API_LIB_SRCS src/xdg-shell-protocol.c)
#add_library(wayland_api STATIC ${WAYLAND_API_LIB_SRCS})
#target_include_directories(wayland_api PUBLIC include)
#set_target_properties(wayland_api PROPERTIES LINKER_LANGUAGE C)

# Bicycle Engine
set(BC_PRIVATE_INC_DIRS include)
set(BC_SRCS src/bicycle-engine.cpp
            src/bc-wayland-client.cpp
            src/bc-wayland-shm.cpp
            src/bc-statistics.cpp
            src/bc-wayland-shm-file.cpp
            src/bc-wayland-shm-pool.cpp)
add_executable(bicycle_engine ${BC_SRCS})
set_target_properties(bicycle_engine PROPERTIES LINKER_LANAGAGE CXX CXX_EXTENSIONS OFF)

target_include_directories(bicycle_engine PRIVATE ${BC_PRIVATE_INC_DIRS})

target_link_libraries(bicycle_engine PRIVATE wayland_api wayland-client rt xkbcommon)
