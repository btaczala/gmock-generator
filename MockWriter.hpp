#ifndef MOCKWRITER_HPP_P2ITIXL9
#define MOCKWRITER_HPP_P2ITIXL9

#include <sstream>
#include <string>

#include "types.hpp"

struct MockWriter {
    MockWriter(const CXXFile& f);
    std::string render();

    const std::string _headerFilePath;
    const CXXFile _file;
};

#endif /* end of include guard: MOCKWRITER_HPP_P2ITIXL9 */
