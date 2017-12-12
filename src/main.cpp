#include <fmt/format.h>
#include <cxxopts.hpp>
#include <fstream>

#include "clangparser.hpp"
#include "fs.hpp"
#include "gmockwriter.hpp"
#include "mockwriter.hpp"

namespace {
void writeFile(const fs::path& path, const std::string& data) {
    if (fs::exists(path)) {
        throw std::runtime_error(
            fmt::format("File {}, already exists", path.string()));
    }

    std::ofstream output{path.c_str()};
    std::copy(data.begin(), data.end(), std::ostream_iterator<char>(output));
}
}  // namespace

int main(int argc, char* argv[]) {
    using namespace cxxopts;

    cxxopts::Options opts{argv[0], "Generates mock files from interface"};
    try {
        std::vector<std::string> inputFiles;
        std::string outputDirectory;
        // clang-format off
        opts.add_options()
            ("positional", "Input files",cxxopts::value(inputFiles))
            ("o,stdout", "Print generated gmock file on stdout")
            ("s,strict", "Use strict mode")
            ("hacks", "include hacks")
            ("d,directory", "Directory where to store generated gmock", cxxopts::value(outputDirectory)->default_value(fs::current_path().string()))
            ("h,help", "Print this help")
            ;
        // clang-format on

        opts.positional_help("header files");
        opts.parse_positional({"positional"});
        auto res = opts.parse(argc, argv);

        if (res.count("h")) {
            std::cout << opts.help({""}) << std::endl;
            return EXIT_SUCCESS;
        }

        for (const auto& inputFile : inputFiles) {
            CXIndex index = clang_createIndex(0, 0);
            ClangParserOptions opts;
            if (res.count("hacks")) {
                opts |= ClangParserOptions::UniquePtrHack;
            }
            ClangParser gen{inputFile, opts};

            MockWriter mw{gen.parse(), GMockImpl{}};
            if (res.count("o")) {
                std::cout << mw.render() << std::endl;
            } else {
                if (!fs::exists(outputDirectory)) {
                    throw std::runtime_error(
                        fmt::format("Output directory {} does not exists",
                                    outputDirectory));
                }

                auto resultPath = fs::path{outputDirectory};
                resultPath /= fs::path{inputFile}.stem();

                resultPath += "Mock.hpp";
                writeFile(resultPath, mw.render());
            }
        }
    } catch (const cxxopts::OptionException& ex) {
        std::cerr << opts.help({""});
    } catch (const std::exception& ex) {
        std::cerr << ex.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
