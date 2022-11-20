QT += charts widgets core testlib

CONFIG += testcase no_testcase_installs

HEADERS += \
    code/mainwidget.h \
    imported/parsearc.h \
    imported/rangeslider.h \
    imported/rapidcsv.h

SOURCES += \
    code/main.cpp \
    code/mainwidget.cpp \
    imported/parsearc.cpp \
    imported/rangeslider.cpp \
    tests/ArcParseUT.cpp

DISTFILES += \
    .gitignore \
    tests/CMakeLists.txt

