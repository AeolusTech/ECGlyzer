#ifndef PARSEARCUT_H
#define PARSEARCUT_H

#include <gtest/gtest.h>

#include <fstream>

#include "parsearc.h"

TEST(TestDummy, ThisShouldPass) {
    EXPECT_TRUE(true);
}

TEST(ParseArcUT, moduleTest)
{
    parsearc("/Users/kkc/private-repos/holter/ECGlyzer/tests/sample.arc", "/Users/kkc/private-repos/holter/");

    const std::string expected_filename = "/Users/kkc/private-repos/holter/ECGlyzer/tests/expected_channel_all.csv";

    std::ifstream data1(expected_filename);
    std::string genfile1((std::istreambuf_iterator<char>(data1)),
                          std::istreambuf_iterator<char>());

    if (!data1.is_open()){
        std::string fail_msg = expected_filename + " file can't be opened...";
        FAIL() << fail_msg.c_str();
    }

    const std::string test_filename = "/Users/kkc/private-repos/holter/channel_all.csv";

    std::ifstream data2(test_filename);
    std::string genfile2((std::istreambuf_iterator<char>(data2)),
                          std::istreambuf_iterator<char>());

    if (!data2.is_open()){
        std::string fail_msg = test_filename + " file can't be opened...";
        FAIL() << fail_msg.c_str();
    }


    ASSERT_EQ(genfile1, genfile2);
}



#endif  // PARSEARCUT_H
