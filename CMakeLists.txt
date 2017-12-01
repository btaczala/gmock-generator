cmake_minimum_required(VERSION 3.8)
project(gmock-generator)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 14)

set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake/modules)

find_package(LibClang REQUIRED)

add_subdirectory(3rdParty)
add_subdirectory(src)

enable_testing()
add_subdirectory(tests)
