
add_executable(generator_tests generator_tests.cpp)
target_link_libraries(generator_tests gtest gg-core stdc++fs)
target_compile_definitions(generator_tests PRIVATE TEST_DIR="${CMAKE_SOURCE_DIR}/test_data" )
add_test(generator_tests generator_tests)
