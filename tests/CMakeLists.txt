include(GoogleTest)

enable_testing()

add_executable(utils_tests main.cpp nms_test.cpp)
target_link_libraries(utils_tests
                          PRIVATE
                            CONAN_PKG::gtest

                          utils)
add_test(NAME utils_tests
         COMMAND utils_tests )
