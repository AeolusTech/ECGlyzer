QT -= gui
include(googletest.pri)


TEMPLATE = lib
CONFIG += c++17 hide_symbols

TARGET = GoogleTest
DEFINES += GOOGLETEST_LIBRARY
DESTDIR = $$PWD/../../bin

INCLUDEPATH += \
    $$PWD/googletest/googletest/include \
    $$PWD/googletest/googlemock/include \
    $$PWD/googletest/googletest \
    $$PWD/googletest/googlemock

SOURCES = \
    $$PWD/googletest/googletest/src/gtest-all.cc \
    $$PWD/googletest/googlemock/src/gmock-all.cc


target.path = $$INSTALL_PREFIX/lib$$LIB_SUFFIX
INSTALLS = target

header_base.files = $$HEADERS_BASE
header_base.path = $$INSTALL_PREFIX/include/log4qt
INSTALLS += header_base

