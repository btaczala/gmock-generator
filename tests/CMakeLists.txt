include(GoogleTest)

function(mock_add_test test_name)
    add_executable(${test_name} ${test_name}.cpp)
    target_link_libraries(${test_name} gtest mg-core fs)
    target_compile_definitions(${test_name} PRIVATE TEST_DIR="${CMAKE_SOURCE_DIR}/test_data" )
    set_target_properties(${test_name} PROPERTIES COMPILE_FLAGS ${LIBCLANG_CXXFLAGS})
    gtest_add_tests(TARGET ${test_name} SOURCES ${test_name}.cpp)

    list(APPEND tests ${test_name})

endfunction()

mock_add_test(generator_tests)
mock_add_test(gmock_generator_tests)
mock_add_test(parser_tests)


if (NOT APPLE)
    SETUP_TARGET_FOR_COVERAGE(
        NAME coverage
        EXECUTABLE ctest
        DEPENDENCIES ${tests}
    )
endif()
