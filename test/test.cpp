#include <gtest/gtest.h>
#include <filesystem>
#include <fstream>
#include <string>

#include "application.hpp"

class FileIOTestq : public ::testing::Test {
protected:
    void SetUp() override {
        std::filesystem::create_directory("got");
    }

    void TearDown() override {
        std::filesystem::remove_all("got");
    }

    std::string readFile(const std::string& filename) {
        std::ifstream file(filename);
        return std::string((std::istreambuf_iterator<char>(file)), 
                         std::istreambuf_iterator<char>());
    }
};

TEST_F(FileIOTestq, TestCase1) {
    std::ifstream input("tests/test_1.txt");
    std::ofstream output("got/ans_1.txt");
    ccms::Application app(input, output);
    app.run();
    
    std::string expected = readFile("answers/ans_1.txt");
    std::string actual = readFile("got/ans_1.txt");

    EXPECT_EQ(expected, actual);
}


TEST_F(FileIOTestq, TestCase2) {
    std::ifstream input("tests/test_2.txt");
    std::ofstream output("got/ans_2.txt");
    ccms::Application app(input, output);
    app.run();
    
    std::string expected = readFile("answers/ans_2.txt");
    std::string actual = readFile("got/ans_2.txt");
    
    EXPECT_EQ(expected, actual);
}


TEST_F(FileIOTestq, TestCase3) {
    std::ifstream input("tests/test_3.txt");
    std::ofstream output("got/ans_3.txt");
    ccms::Application app(input, output);
    app.run();
    
    std::string expected = readFile("answers/ans_3.txt");
    std::string actual = readFile("got/ans_3.txt");
    
    EXPECT_EQ(expected, actual);
}

TEST_F(FileIOTestq, TestCase4) {
    std::ifstream input("tests/test_4.txt");
    std::ofstream output("got/ans_4.txt");
    ccms::Application app(input, output);
    app.run();
    
    std::string expected = readFile("answers/ans_4.txt");
    std::string actual = readFile("got/ans_4.txt");
    
    EXPECT_EQ(expected, actual);
}

TEST_F(FileIOTestq, TestCase5) {
    std::ifstream input("tests/test_5.txt");
    std::ofstream output("got/ans_5.txt");
    ccms::Application app(input, output);
    app.run();
    
    std::string expected = readFile("answers/ans_5.txt");
    std::string actual = readFile("got/ans_5.txt");
    
    EXPECT_EQ(expected, actual);
}

TEST_F(FileIOTestq, TestCase6) {
    std::ifstream input("tests/test_6.txt");
    std::ofstream output("got/ans_6.txt");
    ccms::Application app(input, output);
    app.run();
    
    std::string expected = readFile("answers/ans_6.txt");
    std::string actual = readFile("got/ans_6.txt");
    
    EXPECT_EQ(expected, actual);
}

TEST_F(FileIOTestq, TestCase7) {
    std::ifstream input("tests/test_7.txt");
    std::ofstream output("got/ans_7.txt");
    ccms::Application app(input, output);
    app.run();
    
    std::string expected = readFile("answers/ans_7.txt");
    std::string actual = readFile("got/ans_7.txt");
    
    EXPECT_EQ(expected, actual);
}

TEST_F(FileIOTestq, TestCase8) {
    std::ifstream input("tests/test_8.txt");
    std::ofstream output("got/ans_8.txt");
    ccms::Application app(input, output);
    app.run();
    
    std::string expected = readFile("answers/ans_8.txt");
    std::string actual = readFile("got/ans_8.txt");
    
    EXPECT_EQ(expected, actual);
}

TEST_F(FileIOTestq, TestCase9) {
    std::ifstream input("tests/test_9.txt");
    std::ofstream output("got/ans_9.txt");
    ccms::Application app(input, output);
    app.run();
    
    std::string expected = readFile("answers/ans_9.txt");
    std::string actual = readFile("got/ans_9.txt");
    
    EXPECT_EQ(expected, actual);
}

TEST_F(FileIOTestq, TestCase10) {
    std::ifstream input("tests/test_10.txt");
    std::ofstream output("got/ans_10.txt");
    ccms::Application app(input, output);
    app.run();
    
    std::string expected = readFile("answers/ans_10.txt");
    std::string actual = readFile("got/ans_10.txt");
    
    EXPECT_EQ(expected, actual);
}

TEST_F(FileIOTestq, TestCase11) {
    std::ifstream input("tests/test_11.txt");
    std::ofstream output("got/ans_11.txt");
    ccms::Application app(input, output);
    app.run();
    
    std::string expected = readFile("answers/ans_11.txt");
    std::string actual = readFile("got/ans_11.txt");
    
    EXPECT_EQ(expected, actual);
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
