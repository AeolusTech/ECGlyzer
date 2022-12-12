include(gtest_dependency.pri)

TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG += thread
#CONFIG -= qt

QMAKE_CXXFLAGS += -Wall -Wextra
QMAKE_CFLAGS += -Wall -Wextra -Werror

# gcov
#QMAKE_CXXFLAGS += -fprofile-arcs -ftest-coverage
#QMAKE_CFLAGS += -fprofile-arcs -ftest-coverage
#LIBS += -lgcov

DEFINES += INPUTDIR=\\\"$$PWD/input/\\\"

HEADERS +=     ParseArcUT.h \
    $$PWD/../imported/parsearc.h \

SOURCES +=   main.cpp  \
    $$PWD/../imported/parsearc.cpp

INCLUDEPATH += $$PWD/../imported

#DISTFILES += \
#    input/TestSTDOut_output.txt
