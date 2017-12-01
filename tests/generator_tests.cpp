#include <gtest/gtest.h>
#include <experimental/filesystem>
#include "clangparser.hpp"

namespace fs = std::experimental::filesystem;

const std::string rootTestData = TEST_DIR;

TEST(Generator, non_existent_file_throws) {
    EXPECT_ANY_THROW(ClangParser gen{""});
}

TEST(Generator, reading_file_ok) {
    fs::path testFilePath = fs::path{rootTestData} / "C1.hpp";
    EXPECT_NO_THROW(ClangParser gen{testFilePath.string()});
}

TEST(Generator, c1_hpp) {
    fs::path testFilePath = fs::path{rootTestData} / "C1.hpp";
    ClangParser gen{testFilePath.string()};

    auto parsed = gen.parse();

    ASSERT_EQ(parsed._classes.size(), 1);
    EXPECT_EQ(parsed._classes.at(0)._name, "C1");
    EXPECT_EQ(parsed._classes.at(0)._methods.size(), 3);
    EXPECT_EQ(parsed._classes.at(0)._methods.at(0)._returnType, "void ()");

    ASSERT_EQ(parsed._classes.at(0)._ctors.size(), 1);
    EXPECT_EQ(parsed._classes.at(0)._ctors.at(0)._returnType, "");
    ASSERT_EQ(parsed._classes.at(0)._ctors.at(0)._arguments.size(), 1);
    EXPECT_EQ(parsed._classes.at(0)._ctors.at(0)._arguments.at(0)._type, "int");
}

TEST(Generator, NotificationBackendInterface_hpp) {
    fs::path testFilePath = fs::path{rootTestData} / "NotificationBackendInterface.hpp";
    ClangParser gen{testFilePath.string()};

    auto parsed = gen.parse();

    ASSERT_EQ(parsed._classes.size(), 1);
    EXPECT_EQ(parsed._classes.at(0)._name, "NotificationBackendInterface");
    //EXPECT_EQ(parsed._classes.at(0)._methods.size(), 3);
    //EXPECT_EQ(parsed._classes.at(0)._methods.at(0)._returnType, "void ()");

    //ASSERT_EQ(parsed._classes.at(0)._ctors.size(), 1);
    //EXPECT_EQ(parsed._classes.at(0)._ctors.at(0)._returnType, "");
    //ASSERT_EQ(parsed._classes.at(0)._ctors.at(0)._arguments.size(), 1);
    //EXPECT_EQ(parsed._classes.at(0)._ctors.at(0)._arguments.at(0)._type, "int");
}
int main(int argc, char* argv[]) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
