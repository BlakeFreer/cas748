enable_testing()

file(GLOB test_src *.cpp)
add_executable(test ${test_src})

file(COPY data DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

target_link_libraries(test caslib)

target_link_libraries(test GTest::gtest_main)
include(GoogleTest)
gtest_discover_tests(test)

set(UNIT_TEST test)
add_custom_command(
    TARGET ${UNIT_TEST}
    COMMENT "Run tests"
    POST_BUILD
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    COMMAND ${UNIT_TEST}
)