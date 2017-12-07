include(GoogleTest)

add_executable(generator_tests generator_tests.cpp)
target_link_libraries(generator_tests gtest mg-core fs)
target_compile_definitions(generator_tests PRIVATE TEST_DIR="${CMAKE_SOURCE_DIR}/test_data" )
if(APPLE)
    set_target_properties(generator_tests PROPERTIES COMPILE_FLAGS ${LIBCLANG_CXXFLAGS})
endif()
gtest_add_tests(TARGET generator_tests SOURCES generator_tests.cpp)

add_executable(gmock_generator_tests gmock_generator_tests.cpp)
target_link_libraries(gmock_generator_tests gtest mg-core fs)
target_compile_definitions(gmock_generator_tests PRIVATE TEST_DIR="${CMAKE_SOURCE_DIR}/test_data" )
if(APPLE)
    set_target_properties(gmock_generator_tests PROPERTIES COMPILE_FLAGS ${LIBCLANG_CXXFLAGS})
endif()
gtest_add_tests(TARGET gmock_generator_tests SOURCES gmock_generator_tests.cpp)

if (NOT APPLE)
    SETUP_TARGET_FOR_COVERAGE(
        NAME coverage
        EXECUTABLE ctest
        DEPENDENCIES gmock_generator_tests generator_tests
    )
endif()
