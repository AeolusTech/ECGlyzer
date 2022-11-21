#ifndef PARSEARCUT_H
#define PARSEARCUT_H

#include <QTest>
#include <QString>
#include <QFile>
#include <QDebug>
#include <QTextStream>

#include "parsearc.h"


class ParseArcUT: public QObject
{
    Q_OBJECT

private:
    bool myCondition()
    {
        return true;
    }

private slots:
    void initTestCase()
    {
        qDebug("Called before everything else.");
    }

    void myFirstTest()
    {
        QVERIFY(true); // check that a condition is satisfied
        QCOMPARE(1, 1); // compare two values
    }

    void mySecondTest()
    {
        QVERIFY(myCondition());
        QVERIFY(1 != 2);
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

    void cleanupTestCase()
    {
        qDebug("Called after myFirstTest and mySecondTest.");
    }

    void myBenchmark()
    {
        QString str1 = QLatin1String("This is a test string");
        QString str2 = QLatin1String("This is a test string");

        QCOMPARE(str1.localeAwareCompare(str2), 0);

        QBENCHMARK {
            str1.localeAwareCompare(str2);
        }
    }

};


#endif  // PARSEARCUT_H
