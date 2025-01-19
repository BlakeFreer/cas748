enable_testing()

add_executable(
    test
    helpers.cpp
    test_analyze_stats.cpp
    test_cross_correlation.cpp
    test_csv.cpp
    test_series_operator.cpp
)

file(COPY data DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

target_link_libraries(test caslib)

target_link_libraries(test GTest::gtest_main)
include(GoogleTest)
gtest_discover_tests(test)