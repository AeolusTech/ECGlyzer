QT += core gui testlib widgets

#greaterThan(QT_MAJOR_VERSION, 5): QT += widgets

TARGET = ECGlyzerUnitTests
TEMPLATE = app

CONFIG += console c++11 testcase no_testcase_installs

SOURCES += \
    main.cpp
#    ../imported/parsearc.cpp \

HEADERS += \
    ParseArcUT.h

# INCLUDEPATH += \
#     ../app \
#     ../app/tools \
#     ../app/commands \
#     ../app/interfaces \
#     ../app/shapes \
#     ../app/fileio \
#     ../lib

DESTDIR = $$PWD/../build
