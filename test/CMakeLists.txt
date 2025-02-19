set(GTEST_PATH "${CMAKE_SOURCE_DIR}/third-party/googletest")

if(EXISTS ${GTEST_PATH}/CMakeLists.txt)
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
    add_subdirectory(${GTEST_PATH} ${CMAKE_BINARY_DIR}/gtest)

    enable_testing()

    file(GLOB test_src *.cpp)
    add_executable(test ${test_src})

    file(COPY data DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

    target_link_libraries(test PUBLIC caslib)

    target_link_libraries(test PUBLIC GTest::gtest_main)
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

else()
    message(WARNING "Skipping tests as GoogleTest is not installed.")
endif()