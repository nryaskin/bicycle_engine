add_executable(raw_wayland)
target_sources(raw_wayland PRIVATE main.cpp)
target_compile_features(raw_wayland PUBLIC cxx_std_20)
