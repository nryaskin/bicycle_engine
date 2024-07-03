add_executable(cppgen-sample)
target_sources(cppgen-sample PRIVATE main.cpp)
target_compile_features(cppgen-sample PUBLIC cxx_std_20)
target_include_directories(cppgen-sample PUBLIC
    ${CMAKE_SOURCE_DIR}/libs/cpp/include)
#target_link_libraries(cppgen-sample PUBLIC cppgen)
