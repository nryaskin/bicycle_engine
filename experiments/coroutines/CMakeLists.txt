add_executable(coro-app main.cpp)
target_compile_features(coro-app PUBLIC cxx_std_20)

add_executable(co-await-op co-await.cpp)
target_compile_features(co-await-op PUBLIC cxx_std_20)

add_executable(simple-await simple-await.cpp)
target_compile_features(simple-await PUBLIC cxx_std_20)
