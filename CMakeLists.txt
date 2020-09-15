cmake_minimum_required(VERSION 3.12)
project(NMS)

set(CMAKE_CXX_STANDARD 17)

add_compile_options(-Wall -Wextra -pedantic -Werror "$<$<CONFIG:RELEASE>:-O3>")

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup(TARGETS)

add_subdirectory(nms_utils)
add_subdirectory(tests)

add_executable(main main.cpp)
target_link_libraries(main PRIVATE nms_utils CONAN_PKG::benchmark CONAN_PKG::opencv)

