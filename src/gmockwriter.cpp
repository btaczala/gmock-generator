#include "gmockwriter.hpp"

#include <fmt/format.h>
#include <fmt/ostream.h>

namespace {

const auto kClassFormat =
    R"(#ifndef {class_name}{ifdefSuffix}
#define {class_name}{ifdefSuffix}
{preambule}
#include <gmock/gmock.h>
#include "{header_file}"

class {class_name}{class_mock_suffix} : public {class_name} {{ 
  public:
{class_constructors}
{class_methods} 
}};

#endif // {class_name}{ifdefSuffix})";
}  // namespace

std::ostream& operator<<(std::ostream& os, const std::vector<Arg>& args) {
    if (args.empty()) return os;
    std::for_each(args.begin(), args.end() - 1,
                  [&os](const auto& arg) { os << arg._type << ", "; });

    os << args.back()._type;
    return os;
}

GMockWriter::GMockWriter(const CXXFile& cxxFile) : _file(cxxFile) {}

std::string GMockWriter::render(const Config& cfg) {
    auto classCtorsFormatter = [](const Class& cl) -> std::string {
        std::string buff;

        for (auto& ctor : cl._ctors) {
            buff += fmt::format(
                "    {class_name}{class_mock_suffix}({arguments}) : "
                "{class_name}() {{}}\n",
                fmt::arg("class_name", cl._name),
                fmt::arg("arguments", ctor._arguments),
                fmt::arg("class_mock_suffix", "Mock"));
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
            kClassFormat, fmt::arg("preambule", cfg.preambule()),
            fmt::arg("class_mock_suffix", cfg.mockSuffix()),
            fmt::arg("class_name", _class._name),
            fmt::arg("header_file", _file._filePath),
            fmt::arg("ifdefSuffix", cfg.ifdefSuffix()),
            fmt::arg("class_constructors", classCtorsFormatter(_class)),
            fmt::arg("class_methods", classImplFormatter(_class)));
    }
    // buff.erase(std::unique(buff.begin(), buff.end(), [](auto c1, auto c2) {
    // return c1 == '\n' && c2 == '\n';
    //}));
    return buff;
}
