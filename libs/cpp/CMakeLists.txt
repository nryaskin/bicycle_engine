add_library(cppgen)
target_sources(cppgen PRIVATE src/class.cpp)
# target_sources(cppgen PRIVATE src/class.cpp src/document.cpp src/file.cpp)
target_compile_features(cppgen PUBLIC cxx_std_20)
target_include_directories(cppgen PUBLIC include)

