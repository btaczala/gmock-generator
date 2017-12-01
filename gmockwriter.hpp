#ifndef MOCKWRITER_HPP_P2ITIXL9
#define MOCKWRITER_HPP_P2ITIXL9

#include <sstream>
#include <string>

#include "config.hpp"
#include "types.hpp"

struct GMockWriter {
    GMockWriter(const CXXFile& f);
    std::string render(const Config& cfg = Config{});

    const std::string _headerFilePath;
    const CXXFile _file;
};

#endif /* end of include guard: MOCKWRITER_HPP_P2ITIXL9 */
