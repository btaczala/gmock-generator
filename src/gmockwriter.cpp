#include "gmockwriter.hpp"

#include <fmt/format.h>
#include <fmt/ostream.h>
#include <algorithm>

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

const auto kCtorFormat =
    "    {class_name}{class_mock_suffix}({arguments}) : "
    "{class_name}({ctor_args}) {{}}\n";

const auto kMethodFormat =
    "    MOCK_METHOD{number_of_args}({method_name}, "
    "{return_type}({arguments}));\n";
const auto kConstMethodFormat =
    "    MOCK_CONST_METHOD{number_of_args}({method_name}, "
    "{return_type}({arguments}));\n";

std::string ctorArgs(const std::vector<Arg>& args) {
    if (args.empty()) return "";

    std::stringstream ss;
    std::size_t counter{1};

    std::for_each(args.begin(), args.end() - 1,
                  [&ss, &counter](const Arg& arg) {
                      if (arg._name.empty()) {
                          ss << "arg" << counter++;
                      }

                      ss << ",";
                  });
    auto a = args.back();
    if (a._name.empty()) {
        ss << "arg" << counter++;
    }

    return ss.str();
}
}  // namespace

std::ostream& operator<<(std::ostream& os, const std::vector<Arg>& args) {
    if (args.empty()) return os;
    std::size_t counter{1};
    std::for_each(args.begin(), args.end() - 1,
                  [&os, &counter](const auto& arg) {
                      os << arg._type;
                      if (arg._name.empty()) {
                          os << " arg" << counter++;
                      }
                      os << ", ";
                  });

    auto& arg = args.back();
    os << arg._type << " ";
    if (arg._name.empty()) {
        os << "arg" << counter;
    }

    return os;
}

GMockWriter::GMockWriter(const CXXFile& cxxFile) : _file(cxxFile) {}

std::string GMockWriter::render(const Config& cfg) {
    auto classCtorsFormatter = [](const Class& cl) -> std::string {
        std::string buff;

        for (auto& ctor : cl._ctors) {
            buff +=
                fmt::format(kCtorFormat, fmt::arg("class_name", cl._name),
                            fmt::arg("arguments", ctor._arguments),
                            fmt::arg("ctor_args", ctorArgs(ctor._arguments)),
                            fmt::arg("class_mock_suffix", "Mock"));
        }
        return buff;
    };
    auto classImplFormatter = [](const Class& cl) -> std::string {
        std::string buff;
        for (const auto& m : cl._methods) {
            if (m._const) {
                buff +=
                    fmt::format(kConstMethodFormat,
                                fmt::arg("number_of_args", m._arguments.size()),
                                fmt::arg("method_name", m._name),
                                fmt::arg("return_type", m._returnType),
                                fmt::arg("arguments", m._arguments));
            } else {
                buff +=
                    fmt::format(kMethodFormat,
                                fmt::arg("number_of_args", m._arguments.size()),
                                fmt::arg("method_name", m._name),
                                fmt::arg("return_type", m._returnType),
                                fmt::arg("arguments", m._arguments));
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
    return buff;
}
