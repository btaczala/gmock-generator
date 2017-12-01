#include "MockWriter.hpp"

#include <fmt/format.h>
#include <fmt/ostream.h>
#include <mstch/mstch.hpp>

#include <experimental/filesystem>

namespace {

std::string absolute(const std::string& path) {
    namespace fs = std::experimental::filesystem;

    fs::path filePath{path};
    return fs::canonical(filePath).string();
}

const auto kClassFormat =
    R"({preambule}
#ifdef {class_name}__MOCK_HPP
#define {class_name}__MOCK_HPP

#include <gmock/gmock.h>
#include "{header_file}"

class {class_name} {{ 
  public:
{class_constructors}
{class_methods} 
}};

#endif // {class_name}__MOCK_HPP
)";
}  // namespace

std::ostream& operator<<(std::ostream& os, const std::vector<Arg>& args) {
    std::for_each(args.begin(), args.end() - 1,
                  [&os](const auto& arg) { os << arg._type << ", "; });

    os << args.back()._type;
    return os;
}

MockWriter::MockWriter(const std::string& filename, const CXXFile& cxxFile)
    : _headerFilePath(absolute(filename)), _file(cxxFile) {}

std::string MockWriter::render() {
    auto classCtorsFormatter = [](const Class& cl) -> std::string {
        std::string buff;

        for (auto& ctor : cl._ctors) {
            buff += fmt::format("    {}Mock({}) : {}() {{}}\n", cl._name,
                                ctor._arguments, cl._name);
        }
        return buff;
    };
    auto classImplFormatter = [](const Class& cl) -> std::string {
        std::string buff;
        for (const auto& m : cl._methods) {
            buff += fmt::format("    MOCK_METHOD{}({}, {});\n",
                                m._arguments.size(), m._name, m._returnType);
        }
        return buff;
    };

    std::string buff;
    for (const auto& _class : _file._classes) {
        buff += fmt::format(
            kClassFormat, fmt::arg("preambule", ""),
            fmt::arg("class_name", _class._name + "Mock"),
            fmt::arg("header_file", _headerFilePath),
            fmt::arg("class_constructors", classCtorsFormatter(_class)),
            fmt::arg("class_methods", classImplFormatter(_class)));
    }
    return buff;
}
