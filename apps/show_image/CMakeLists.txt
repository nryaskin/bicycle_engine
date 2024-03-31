set(SHOW_IMAGE_SRCS main.cpp)
add_executable(show_image ${SHOW_IMAGE_SRCS})
target_link_libraries(show_image PRIVATE bicycle_engine)
target_compile_features(show_image PUBLIC cxx_std_20)
