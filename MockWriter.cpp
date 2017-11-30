#include "MockWriter.hpp"

#include <fmt/format.h>
#include <mstch/mstch.hpp>

namespace {

struct FileWriter : public mstch::object {
    FileWriter(const CXXFile& file) : _file(file) {
        register_methods(this, {{"classes", &FileWriter::classes},
                                {"class_name", &FileWriter::className}});
    }

    mstch::node className() { return std::string{"c1"}; }

    mstch::node classes() {
        mstch::array a{std::string{"test"}};
        return a;
    }

   private:
    CXXFile _file;
};
}  // namespace

std::string MockWriter::render() {
    std::stringstream ss;
    std::string view{"{{#classes}}<b>{{class_name}}</b>: {{.}}\n{{/classes}}"};
    const auto context = std::make_shared<FileWriter>(_file);

    return mstch::render(view, context);
}
