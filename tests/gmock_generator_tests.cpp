#include <fmt/format.h>
#include <gtest/gtest.h>
#include <fstream>

#include "clangparser.hpp"
#include "config.hpp"
#include "fs.hpp"

#include "gmockwriter.hpp"

const std::string rootTestData = TEST_DIR;

std::string readFile(const fs::path& p) {
    if (!fs::exists(p)) {
        throw std::runtime_error(
            fmt::format("File {} does not exists", p.string()));
    }
    std::string buffer;
    std::ifstream file{p.c_str(), std::ios::in};

    file >> std::noskipws;
    std::copy(std::istream_iterator<char>(file), std::istream_iterator<char>(),
              std::back_inserter(buffer));
    return buffer;
}
using StringPair = std::pair<std::string, std::string>;

struct GmockGeneratorTest : public ::testing::TestWithParam<StringPair> {};

TEST_P(GmockGeneratorTest, MutlipleNamespaces) {
    Config cfg;

    const auto files = GetParam();

    fs::path testFilePath = fs::path{rootTestData} / files.first;
    fs::path expectedFile = fs::path{rootTestData} / "expected" / files.second;
    ClangParser p{testFilePath.string()};

    GMockWriter gw{p.parse()};

    auto output = gw.render(cfg);
    EXPECT_EQ(output, readFile(expectedFile));
}

INSTANTIATE_TEST_CASE_P(
    Mocks, GmockGeneratorTest,
    ::testing::Values(
        std::make_pair("MultipleNamespaces.hpp", "MultipleNamespacesMock.hpp"),
        std::make_pair("NoNamespace.hpp", "NoNamespaceMock.hpp")));

int main(int argc, char* argv[]) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
