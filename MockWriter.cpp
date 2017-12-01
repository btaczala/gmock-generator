#include "MockWriter.hpp"

#include <fmt/format.h>
#include <fmt/ostream.h>
#include <mstch/mstch.hpp>

namespace {

const auto kClassFormat =
    R"({preambule}
#ifndef {class_name}__MOCK_HPP
#define {class_name}__MOCK_HPP

#include <gmock/gmock.h>
#include "{header_file}"

class {class_name}Mock {{ 
  public:
{class_constructors}
{class_methods} 
}};

#endif // {class_name}__MOCK_HPP
)";
}  // namespace

std::ostream& operator<<(std::ostream& os, const std::vector<Arg>& args) {
    if (args.empty()) return os;
    std::for_each(args.begin(), args.end() - 1,
                  [&os](const auto& arg) { os << arg._type << ", "; });

    os << args.back()._type;
    return os;
}

MockWriter::MockWriter(const CXXFile& cxxFile) : _file(cxxFile) {}

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
            if (m._const) {
                buff += fmt::format("    MOCK_CONST_METHOD{}({}, {}({}));\n",
                                    m._arguments.size(), m._name, m._returnType,
                                    m._arguments);
            } else {
                buff += fmt::format("    MOCK_METHOD{}({}, {}({}));\n",
                                    m._arguments.size(), m._name, m._returnType,
                                    m._arguments);
            }
        }
        return buff;
    };

    std::string buff;
    for (const auto& _class : _file._classes) {
        buff += fmt::format(
            kClassFormat, fmt::arg("preambule", ""),
            fmt::arg("class_name", _class._name),
            fmt::arg("header_file", _file._filePath),
            fmt::arg("class_constructors", classCtorsFormatter(_class)),
            fmt::arg("class_methods", classImplFormatter(_class)));
    }
    return buff;
}
