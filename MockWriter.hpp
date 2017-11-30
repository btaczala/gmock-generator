#ifndef MOCKWRITER_HPP_P2ITIXL9
#define MOCKWRITER_HPP_P2ITIXL9

#include <sstream>
#include <string>

#include "generator.hpp"

struct MockWriter {
    MockWriter(const CXXFile& f) : _file(f) {}
    std::string render();

    const CXXFile _file;
};

#endif /* end of include guard: MOCKWRITER_HPP_P2ITIXL9 */
