cmake_minimum_required(VERSION 3.25..3.27)

project(BicycleEngine
        VERSION 0.1
        DESCRIPTION "Different graphics api's playground"
        LANGUAGES CXX C)

add_compile_options("-fdiagnostics-show-template-tree")
include(cmake/checks.cmake)
check_not_source()

set(BUILD_EXPERIMENTS ON CACHE BOOL "Build experiments libraries?")

add_subdirectory(libs)
add_subdirectory(src)
add_subdirectory(apps)
if (BUILD_EXPERIMENTS)
    add_subdirectory(experiments)
endif()
