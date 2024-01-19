if (DEFINED ENV{XDG_SESSION_TYPE})
    # This is not the best way to search for wayland
    message("Found wayland session type")
    find_program(WAYLAND_SCANNER wayland-scanner)
    if (NOT WAYLAND_SCANNER) 
        message(FATAL_ERROR "WAYLAND_SCANNER not found")
    endif()
    set (WAYLAND_SCHEMA "/usr/share/wayland/wayland.xml")
    if (NOT EXISTS "${WAYLAND_SCHEMA}")
        message(FATAL_ERROR "COULDN't FIND wayland.xml")
    endif()
    set (XDG_SHELL_SCHEMA "/usr/share/wayland-protocols/stable/xdg-shell/xdg-shell.xml")

    if (NOT EXISTS "${XDG_SHELL_SCHEMA}")
        message(FATAL_ERROR "COULDN't FIND xdg-shell.xml")
    endif()
else()
    message("XDG_SESSION_TYPE IS NOT DEFINED SO NOT WAYLAND!")
endif()

set(WAYLAND_HEADERS)
set(WAYLAND_SRCS)

function(GENERATE_WAYLAND_CODE)
    set(options "")
    set(oneValueArgs GEN_NAME SCHEMA_PATH)
    set(multiValueArgs "")
    cmake_parse_arguments(GENERATE_WAYLAND "${options}" "${oneValueArgs}"
        "${multiValueArgs}" ${ARGN})

    set(HEADER ${GENERATE_WAYLAND_GEN_NAME}_header.h)
    set(SRC ${GENERATE_WAYLAND_GEN_NAME}_src.c)

    set(WAYLAND_HEADERS "${WAYLAND_HEADERS}" "${HEADER}" PARENT_SCOPE)
    set(WAYLAND_SRCS "${WAYLAND_SRCS}" "${SRC}" PARENT_SCOPE)

    add_custom_command(OUTPUT ${HEADER}
        COMMAND ${WAYLAND_SCANNER} client-header ${GENERATE_WAYLAND_SCHEMA_PATH} ${HEADER}
        DEPEND ${GENERATE_WAYLAND_SCHEMA_PATH})
    
    add_custom_command(OUTPUT ${SRC}
        COMMAND ${WAYLAND_SCANNER} private-code ${GENERATE_WAYLAND_SCHEMA_PATH} ${SRC}
        DEPENDS ${GENERATE_WAYLAND_SCHEMA_PATH})
endfunction()

generate_wayland_code(GEN_NAME wayland_client SCHEMA_PATH "${WAYLAND_SCHEMA}")
generate_wayland_code(GEN_NAME xdg_client SCHEMA_PATH "${XDG_SHELL_SCHEMA}")

add_library(wayland_api STATIC)
target_sources(wayland_api PRIVATE "${WAYLAND_SRCS}")
target_include_directories(wayland_api PUBLIC "${CMAKE_CURRENT_BINARY_DIR}")
set_target_properties(wayland_api PROPERTIES LINKER_LANGUAGE C)
