#include <cxxopts.hpp>

#include "MockWriter.hpp"
#include "clangparser.hpp"

int main(int argc, char* argv[]) {
    using namespace cxxopts;

    try {
        cxxopts::Options opts{argv[0], "Generates mock files from interface"};

        std::vector<std::string> inputFiles;
        // clang-format off
        opts.add_options()
            ("positional", "Input files",cxxopts::value<std::vector<std::string>>(inputFiles))
            ("h,help", "Print this help")
            ;
        // clang-format on

        opts.parse_positional({"positional"});
        auto res = opts.parse(argc, argv);

        if (res.count("h")) {
            std::cout << opts.help({""}) << std::endl;
            return EXIT_SUCCESS;
        }

        for (const auto& inputFile : inputFiles) {
            std::cout << "Parsing " << inputFile << std::endl;
            CXIndex index = clang_createIndex(0, 0);
            ClangParser gen{inputFile};

            std::cout << "\n\n\n"
                      << MockWriter{gen.parse()}.render() << std::endl;
        }
    } catch (const std::exception& ex) {
        std::cerr << ex.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
