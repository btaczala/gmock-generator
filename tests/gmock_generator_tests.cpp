#include <fmt/format.h>
#include <gtest/gtest.h>
#include <fstream>

#include "clangparser.hpp"
#include "config.hpp"
#include "fs.hpp"

#include "gmockwriter.hpp"
#include "mockwriter.hpp"

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

std::vector<std::string> split_string(const std::string& str,
                                      const std::string& delimiter) {
    std::vector<std::string> strings;

    std::string::size_type pos = 0;
    std::string::size_type prev = 0;
    while ((pos = str.find(delimiter, prev)) != std::string::npos) {
        strings.push_back(str.substr(prev, pos - prev));
        prev = pos + 1;
    }

    // To get the last substring (or only, if delimiter is not found)
    strings.push_back(str.substr(prev));

    return strings;
}

bool compareTwoStringsWithSpecial(const std::string& lhs,
                                  const std::string& rhs) {
    const auto left = split_string(lhs, "\n");
    const auto right = split_string(rhs, "\n");

    if (left.size() != right.size()) return false;

    auto rightIter = right.begin();
    bool equal = true;

    std::for_each(left.begin(), left.end(),
                  [&equal, &rightIter](const std::string& line) {
                      auto rightLine = *rightIter;

                      if (line.find("// SKIP") == std::string::npos &&
                          rightLine.find("// SKIP") == std::string::npos) {
                          if (line != rightLine) {
                              equal = false;
                          }
                      }

                      rightIter++;
                  });

    return equal;
}

TEST_P(GmockGeneratorTest, MutlipleNamespaces) {
    Config cfg;

    const auto files = GetParam();

    fs::path testFilePath = fs::path{rootTestData} / files.first;
    fs::path expectedFile = fs::path{rootTestData} / "expected" / files.second;
    ClangParser p{testFilePath.string()};

    MockWriter gw{p.parse(), GMockImpl{}};

    auto output = gw.render(cfg);
    const bool bValue =
        compareTwoStringsWithSpecial(output, readFile(expectedFile));
    EXPECT_TRUE(bValue);
}

INSTANTIATE_TEST_CASE_P(
    Mocks, GmockGeneratorTest,
    ::testing::Values(
        std::make_pair("MultipleNamespaces.hpp", "MultipleNamespacesMock.hpp"),
        std::make_pair("C1.hpp", "C1Mock.hpp"),
        std::make_pair("NoNamespace.hpp", "NoNamespaceMock.hpp")));

int main(int argc, char* argv[]) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
