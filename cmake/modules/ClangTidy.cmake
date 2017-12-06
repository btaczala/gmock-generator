find_program(
    CLANG_TIDY_FOUND
    NAMES "clang-tidy"
    DOC "Path to clang-tidy executable"
)
if(CLANG_TIDY_FOUND)
    message(STATUS "Clang tidy found ${CLANG_TIDY_FOUND}")
    set(DO_CLANG_TIDY "${CLANG_TIDY_FOUND}" "-checks=performance-*,modernize-*,cppcoreguidelines-*")
endif()

set(CLANG_TIDY ${CLANG_TIDY_FOUND})
