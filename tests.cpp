#include <gtest/gtest.h>
#include "MD5.h"
#include <string>
#include <sstream>

TEST(MD5Test, CorrectHashCalculation) {
    EXPECT_EQ(md5("abc"), "900150983cd24fb0d6963f7d28e17f72");
    EXPECT_EQ(md5(""), "d41d8cd98f00b204e9800998ecf8427e");
    EXPECT_EQ(md5("Hello, World!"), "65a8e27d8879283831b664bd8b7f0ad4");
}

TEST(RunTest, MatchFound_abc) {
    is_found = false;
    is_paused = false;
    hash = "900150983cd24fb0d6963f7d28e17f72";
    charset = "abcd123$%@#";
    std::ostringstream out;
    run(out);
    EXPECT_EQ(out.str(), "A match was found: abc\n");
}

TEST(RunTest, MatchFound_abc123) {
    is_found = false;
    is_paused = false;
    hash = "e99a18c428cb38d5f260853678922e03"; // MD5("abc123")
    charset = "abcdef123";
    std::ostringstream out;
    run(out);
    EXPECT_EQ(out.str(), "A match was found: abc123\n");
}

TEST(RunTest, NoMatchFound) {
    is_found = false;
    is_paused = false;
    hash = "5d41402abc4b2a76b9719d911017c592"; // MD5("hello")
    charset = "xyz";
    std::ostringstream out;
    run(out);
    EXPECT_EQ(out.str(), "There are no matches\n");
}

TEST(RunTest, EmptyCharset) {
    is_found = false;
    is_paused = false;
    hash = "900150983cd24fb0d6963f7d28e17f72"; // MD5("abc")
    charset = "";
    std::ostringstream out;
    run( out);
    EXPECT_EQ(out.str(), "There are no matches\n");
}

TEST(RunTest, MatchFound_123) {
    is_found = false;
    is_paused = false;
    hash = "202cb962ac59075b964b07152d234b70"; // MD5("123")
    charset = "012345";
    std::ostringstream out;
    run(out);
    EXPECT_EQ(out.str(), "A match was found: 123\n");
}
TEST(RunTest, LongCharsetMatch) {
    is_found = false;
    is_paused = false;
    hash = "900150983cd24fb0d6963f7d28e17f72"; // MD5("abc")
    charset = "abcdefghijklmnopqrstuvwxyz1234567890!@#$%^&*()";
    std::ostringstream out;
    run(out);
    EXPECT_EQ(out.str(), "A match was found: abc\n");
}