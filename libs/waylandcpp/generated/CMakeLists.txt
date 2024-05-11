add_library(waylandcpp-generated-obj OBJECT)

set(WAYLAND_XML_PATH "/usr/share/wayland/wayland.xml" CACHE STRING "Path to wayland.xml file")
set(WAYLANDCPP_GENERATOR "${CMAKE_SOURCE_DIR}/bin/waylandcpp-generator" CACHE
    PATH "Path to waylandcpp-generator")


function(generate_list XML_CONFIG OUTPATH)
    message("GENERATE values config: ${XML_CONFIG}, path: ${OUTPATH}")
    execute_process(COMMAND ${WAYLANDCPP_GENERATOR} --config ${WAYLAND_XML_PATH}
        --output ${CMAKE_CURRENT_BINARY_DIR} --dry-run)
endfunction()

generate_list(${WAYLAND_XML_PATH} ${CMAKE_CURRENT_BINARY_DIR})
include(${CMAKE_CURRENT_BINARY_DIR}/files.cmake)

# TODO: Think maybe I can generate file list on the go.
add_custom_command(OUTPUT ${GENERATED_SOURCES} ${GENERATED_HEADERS}
    COMMAND ${WAYLANDCPP_GENERATOR}
    ARGS --config ${WAYLAND_XML_PATH} --output ${CMAKE_CURRENT_BINARY_DIR}
    DEPENDS ${WAYLAND_XML_PATH} ${CMAKE_CURRENT_BINARY_DIR}/files.cmake)


target_sources(waylandcpp-generated-obj PRIVATE ${GENERATED_SOURCES})
target_compile_features(waylandcpp-generated-obj PUBLIC cxx_std_20)
target_include_directories(waylandcpp-generated-obj PUBLIC
    ${CMAKE_CURRENT_BINARY_DIR}/include)
target_link_libraries(waylandcpp-generated-obj PUBLIC waylandcpp-common)
