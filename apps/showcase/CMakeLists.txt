set(SHOWCASE_SRCS showcase.cpp)
add_executable(showcase ${SHOWCASE_SRCS})
target_link_libraries(showcase PRIVATE bicycle_engine)
target_compile_features(showcase PUBLIC cxx_std_20)
