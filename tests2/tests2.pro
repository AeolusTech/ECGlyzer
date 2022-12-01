QT += testlib

TEMPLATE = app
TARGET = ECGlyzerGoogleTests
CONFIG += console


INCLUDEPATH += \
    $$PWD/../googletest/googletest/googletest/include \
    $$PWD/../googletest/googletest/googlemock/include

#INCLUDEPATH += ../googletest
#LIBS += $$DESTDIR/libGoogleTest.a
#PRE_TARGETDEPS += $$DESTDIR/libGoogleTest.a

SOURCES += \
    $$PWD/../googletest/googletest/googletest/src/gtest-all.cc \
    $$PWD/../googletest/googletest/googlemock/src/gmock-all.cc \
    DummyTest.cpp

