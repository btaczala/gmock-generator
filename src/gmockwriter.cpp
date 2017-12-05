#include "gmockwriter.hpp"

#include <fmt/format.h>
#include <fmt/ostream.h>
#include <algorithm>

namespace {

const auto kFileFormat =
    R"(#ifndef {class_name}{ifdefSuffix}
#define {class_name}{ifdefSuffix}
{preambule}
#include <gmock/gmock.h>
#include "{header_file}"

{namespaces}
{classes}
{namespaces_end}

#endif // {class_name}{ifdefSuffix}
)";

const auto kClassFormat =
    R"(class {class_name}{class_mock_suffix} : public {class_name} {{
public :
{class_constructors} 
{class_methods}
}};)";
const auto kNamespaceFormat =
    R"(namespace {namespace_name} {{
)";

const auto kEndNamespaceFormat =
    R"(}} // namespace {namespace_name}
)";

const auto kCtorFormat =
    R"(    {class_name}{class_mock_suffix}({arguments}) : {class_name}({ctor_args}) {{}}
)";

const auto kMethodFormat =
    R"(    MOCK_METHOD{number_of_args}({method_name}, {return_type}({arguments}));
)";
const auto kConstMethodFormat =
    R"(    MOCK_CONST_METHOD{number_of_args}({method_name}, {return_type}({arguments}));
)";

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

    std::string classesText;

    const auto& mainClass = _file._namespaces.back()._classes.at(0);

    std::string nsBuffer;
    std::string endNamespaceBuffer;
    std::for_each(_file._namespaces.rbegin(), _file._namespaces.rend(),
                  [&endNamespaceBuffer](const auto& ns) {
                      endNamespaceBuffer +=
                          fmt::format(kEndNamespaceFormat,
                                      fmt::arg("namespace_name", ns._name));
                  });
    for (const auto& ns : _file._namespaces) {
        nsBuffer +=
            fmt::format(kNamespaceFormat, fmt::arg("namespace_name", ns._name));

        for (const auto& clazz : ns._classes) {
            classesText += fmt::format(
                kClassFormat, fmt::arg("class_name", clazz._name),
                fmt::arg("class_mock_suffix", cfg.mockSuffix()),
                fmt::arg("class_constructors", classCtorsFormatter(clazz)),
                fmt::arg("class_methods", classImplFormatter(clazz)));
        }
    }

    return fmt::format(kFileFormat, fmt::arg("preambule", cfg.preambule()),
                       fmt::arg("class_name", mainClass._name),
                       fmt::arg("header_file", _file._filePath),
                       fmt::arg("ifdefSuffix", cfg.ifdefSuffix()),
                       fmt::arg("classes", classesText),
                       fmt::arg("namespaces_end", endNamespaceBuffer),
                       fmt::arg("namespaces", nsBuffer));
}
