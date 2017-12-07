#ifndef MOCKWRITER_HPP_P2ITIXL9
#define MOCKWRITER_HPP_P2ITIXL9

#include <memory>
#include <sstream>
#include <string>

#include "config.hpp"
#include "types.hpp"

template <typename T>
std::string includeFiles(const T& t) {
    return "";
}

struct MockWriter {
    template <typename T>
    MockWriter(const CXXFile& f, const T& t = T{})
        : _file(f), _impl(new model_t<T>()) {}
    std::string render(const Config& cfg = Config{});

    const std::string _headerFilePath;
    const CXXFile _file;

   private:
    struct concept_t {
        virtual std::string do_includeFiles() const noexcept = 0;
    };

    template <typename T>
    struct model_t final : public concept_t {
        virtual std::string do_includeFiles() const noexcept override {
            return includeFiles(_data);
        }
        T _data;
    };

    std::unique_ptr<concept_t> _impl;
};

#endif /* end of include guard: MOCKWRITER_HPP_P2ITIXL9 */
