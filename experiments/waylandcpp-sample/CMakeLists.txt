add_executable(waylandcpp-sample)
target_sources(waylandcpp-sample PRIVATE main.cpp)
target_compile_features(waylandcpp-sample PUBLIC cxx_std_20)
target_link_libraries(waylandcpp-sample PRIVATE waylandcpp)
