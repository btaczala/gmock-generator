#include <gtest/gtest.h>

#include "clangparser.hpp"
#include "fs.hpp"

template <typename T, typename V>
bool contains(T&& t, V&& v) {
    return std::find(std::begin(t), std::end(t), v) != std::end(t);
}

const std::string rootTestData = TEST_DIR;

TEST(ClangParser, parse) {
    fs::path testFilePath = fs::path{rootTestData} / "C1.hpp";

    ClangParser parser{testFilePath.string()};

    const auto file = parser.parse();
    ASSERT_EQ(file._namespaces.size(), 1);
    EXPECT_EQ(file._namespaces.at(0)._classes.size(), 1);
    EXPECT_EQ(file._namespaces.at(0)._classes.at(0)._methods.size(), 3);

    std::vector<std::string> returnTypes;
    const auto& methods = file._namespaces.at(0)._classes.at(0)._methods;

    std::transform(methods.begin(), methods.end(),
                   std::back_inserter(returnTypes),
                   [](const auto& method) { return method._returnType; });

    EXPECT_TRUE(contains(returnTypes, "std::shared_ptr<std::string>"));
}

int main(int argc, char* argv[]) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
