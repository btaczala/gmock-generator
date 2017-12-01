add_library(gg-core clangparser.cpp gmockwriter.cpp config.cpp)
target_include_directories(gg-core PUBLIC ${CLANG_INCLUDE_DIRS} ${LLVM_INCLUDE_DIR} ${mstch_INCLUDE_DIR} ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(gg-core ${LIBCLANG_LIBRARIES} fmt stdc++fs)

add_executable(gmock-generator main.cpp)
target_include_directories(gmock-generator PUBLIC ${CLANG_INCLUDE_DIRS} ${LLVM_INCLUDE_DIR} ${mstch_INCLUDE_DIR})
target_link_libraries(gmock-generator cxxopts ${LIBCLANG_LIBRARIES} gg-core)

