cmake_minimum_required(VERSION 3.8)
project(gmock-generator)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 14)

set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake/modules)

find_package(LibClang REQUIRED)

add_subdirectory(3rdParty)

add_library(gg-core clangparser.cpp MockWriter.cpp)
target_include_directories(gg-core PUBLIC ${CLANG_INCLUDE_DIRS} ${LLVM_INCLUDE_DIR} ${mstch_INCLUDE_DIR} ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(gg-core ${LIBCLANG_LIBRARIES} fmt mstch stdc++fs)

add_executable(gmock-generator main.cpp)
target_include_directories(gmock-generator PUBLIC ${CLANG_INCLUDE_DIRS} ${LLVM_INCLUDE_DIR} ${mstch_INCLUDE_DIR})
target_link_libraries(gmock-generator cxxopts ${LIBCLANG_LIBRARIES} fmt mstch gg-core)

enable_testing()
add_subdirectory(tests)
