add_executable(event_loop_1)
target_sources(event_loop_1 PRIVATE event_loop.cpp)
target_compile_features(event_loop_1 PUBLIC cxx_std_20)
target_link_libraries(event_loop_1 PRIVATE wayland-api rt xkbcommon)

add_executable(event_loop_2)
target_sources(event_loop_2 PRIVATE event_loop.cpp)
target_compile_features(event_loop_2 PUBLIC cxx_std_20)
target_link_libraries(event_loop_2 PRIVATE wayland-api rt xkbcommon)
target_compile_definitions(event_loop_2 PUBLIC EVENT_LOOP)

add_executable(event_loop_3)
target_sources(event_loop_3 PRIVATE event_loop.cpp)
target_compile_features(event_loop_3 PUBLIC cxx_std_20)
target_link_libraries(event_loop_3 PRIVATE wayland-api rt xkbcommon)
target_compile_definitions(event_loop_3 PUBLIC ROUNDTRIP)
