find_package(GTest REQUIRED)

add_executable(Tests
 "src/test_utils.cpp"
 "src/main/executor_test.cpp"
 )

target_include_directories(Tests PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/../src
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_BINARY_DIR}/..
)

target_link_libraries(Tests PRIVATE
    GTest::gtest
    GTest::gtest_main
    SimpleBackendServiceCppLib
)

set_property(TARGET Tests PROPERTY CXX_STANDARD 20)

# Register with CTest / Visual Studio Test Explorer
add_test(NAME Tests COMMAND Tests)
