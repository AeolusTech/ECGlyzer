#ifndef PARSEARCUT_H
#define PARSEARCUT_H

#include <gtest/gtest.h>

TEST(TestDummy, ThisShouldFail) {
    EXPECT_TRUE(false);
}

//    void moduleTest()
//    {
//        parsearc("/Users/kkc/private-repos/holter/ECGlyzer/tests/sample.arc", "/Users/kkc/private-repos/holter/ECGlyzer/tests");

//        const std::string expected_filename = "/Users/kkc/private-repos/holter/ECGlyzer/tests/expected_channel_all.csv";

//        QFile data1(expected_filename.c_str());
//        if (!data1.open(QIODevice::ReadOnly | QIODevice::Text)){
//            std::string fail_msg = expected_filename + "file can't be opened...";
//            QFAIL(fail_msg.c_str());
//        }

//        const std::string test_filename = "channel_all.csv";

//        QFile data2(test_filename.c_str());
//        if (!data2.open(QIODevice::ReadOnly | QIODevice::Text)){
//            std::string fail_msg = test_filename + "file can't be opened...";
//            QFAIL(fail_msg.c_str());
//        }


//        QTextStream in1(&data1), in2(&data2);

//        while ( !in1.atEnd() && !in2.atEnd() ) {
//            QString num1 = in1.readLine();
//            QString num2 = in2.readLine();
//            if ( num1 != num2 ) {
//                std::string fail_msg = "different lines :(:\n\nExpected:\n" + num1.toStdString() + "\nGot:\n" + num2.toStdString();
//                QFAIL(fail_msg.c_str());
//            }
//        }
//    }



#endif  // PARSEARCUT_H
