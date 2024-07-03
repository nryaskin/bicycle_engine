function(check_not_source)
    file(TO_CMAKE_PATH "${PROJECT_BINARY_DIR}/CMakeLists.txt" LOC_PATH)
    message ("LOC_PATH IS '${LOC_PATH}'")
    if (EXISTS ${LOC_PATH})
        message(FATAL_ERROR "You cannot build in a source directory (or any directory with a CMakeLists.txt file). Please make a build subdirectory. Feel free to remove CMakeCache.txt and CMakeFiles.")
    endif()
include(cmake/checks.cmake)
endfunction()
