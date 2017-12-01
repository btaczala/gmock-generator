include(GoogleTest)

add_executable(generator_tests generator_tests.cpp)
target_link_libraries(generator_tests gtest gg-core stdc++fs)
target_compile_definitions(generator_tests PRIVATE TEST_DIR="${CMAKE_SOURCE_DIR}/test_data" )
gtest_add_tests(TARGET generator_tests SOURCES generator_tests.cpp)
