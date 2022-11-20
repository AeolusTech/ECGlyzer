QT       += core gui
greaterThan(QT_MAJOR_VERSION, 5): QT += widgets

TARGET = ECGlyzerUnitTests
TEMPLATE = app

CONFIG += console
CONFIG += c++11

SOURCES += \
    ../imported/parsearc.cpp \
    ParseArcUT.cpp

HEADERS += \
    ../imported/parsearc.h

# INCLUDEPATH += \
#     ../app \
#     ../app/tools \
#     ../app/commands \
#     ../app/interfaces \
#     ../app/shapes \
#     ../app/fileio \
#     ../lib

DESTDIR = $$PWD/../build