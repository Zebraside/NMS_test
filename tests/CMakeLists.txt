include(GoogleTest)

enable_testing()

add_executable(nms_utils_tests
                  main.cpp
                  nms_test.cpp)

target_link_libraries(nms_utils_tests
                          PRIVATE
                              CONAN_PKG::gtest
                              nms_utils)

add_test(NAME nms_utils_tests
         COMMAND nms_utils_tests)
