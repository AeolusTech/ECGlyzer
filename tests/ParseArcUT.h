#ifndef PARSEARCUT_H
#define PARSEARCUT_H

#include <gtest/gtest.h>

#include <fstream>
#include <filesystem>

#include "parsearc.h"


testing::AssertionResult AlgorithmGood(const std::string& expected, const std::string& got) {
  if (expected == got)
    return testing::AssertionSuccess() << " algorithm is good";
  else
    return testing::AssertionFailure() << " algorithm is bad";
}

TEST(ParseArcUT, moduleTest)
{
    auto readData = ReadDataFromArc("/Users/kkc/private-repos/holter/ECGlyzer/tests/sample.arc");
    ASSERT_FALSE(readData.empty());



    auto stdVectorChannelToString = [] (std::vector<CHANNEL>&& channels) {
        std::string retVal;
        int nrecords = 10240;
        int nchannels = 12;
        int frequency = 1024;
        std::string CSVSEPARATOR = ",";

        for (int i = 0; i < nrecords; i++) {
            for (int j = 0; j < nchannels; j++) {
                if (j == 0) {
                    retVal += std::to_string(i/(double)frequency);
                    retVal += CSVSEPARATOR;
                }
                retVal += channels[j].v[i];
                if (j == nchannels-1) {
                    retVal += "\n";
                } else {
                    retVal += CSVSEPARATOR;
                }
            }
        }

        return retVal;
    };

    std::string resultData = stdVectorChannelToString(std::move(readData));
    ASSERT_FALSE(resultData.empty());

    std::ifstream t("/Users/kkc/private-repos/holter/ECGlyzer/tests/EXPECTED_PARSED_STRING.txt");
    std::string EXPECTED_PARSED_STRING((std::istreambuf_iterator<char>(t)),
                     std::istreambuf_iterator<char>());

    EXPECT_TRUE(AlgorithmGood(EXPECTED_PARSED_STRING, resultData));

    std::ofstream out("output.txt");
    out << resultData;
    out.close();
}



#endif  // PARSEARCUT_H
