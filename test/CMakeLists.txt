enable_testing()

add_executable(
    test
    helpers.cpp
    test_analyze_stats.cpp
    test_cross_correlation.cpp
    test_csv.cpp
    test_series_constructor.cpp
    test_series_create.cpp
    test_series_operator.cpp
)

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