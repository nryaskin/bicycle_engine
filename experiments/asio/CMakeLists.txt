find_package(Boost REQUIRED CONFIG COMPONENTS system)

add_executable(simple_callback)
target_sources(simple_callback PRIVATE simple_callback.cpp)
target_compile_features(simple_callback PUBLIC cxx_std_20)
target_link_libraries(simple_callback PRIVATE Boost::system)

add_executable(completion_token)
target_sources(completion_token PRIVATE completion_token.cpp)
target_compile_features(completion_token PUBLIC cxx_std_20)
target_link_libraries(completion_token PRIVATE Boost::system)
