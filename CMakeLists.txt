cmake_minimum_required(VERSION 3.25..3.27)

project(BicycleEngine
        VERSION 0.1
        DESCRIPTION "Different graphics api's playground"
        LANGUAGES CXX C)

include(cmake/checks.cmake)
check_not_source()

add_subdirectory(libs)
add_subdirectory(src)
add_subdirectory(apps)
add_subdirectory(experiments)
