isEmpty(GOOGLEBENCHMARK_DIR):GOOGLEBENCHMARK_DIR=$$(GOOGLEBENCHMARK_DIR)

isEmpty(GOOGLEBENCHMARK_DIR) {
    warning("Using benchmark src dir specified at Qt Creator wizard")
    message("set GOOGLEBENCHMARK_DIR as environment variable or qmake variable to get rid of this message")
    GOOGLEBENCHMARK_DIR = $$PWD/../submodules/benchmark
}

!isEmpty(GOOGLEBENCHMARK_DIR): {
    GOOGLEBENCHMARK_SRCDIR = $$GOOGLEBENCHMARK_DIR
}

requires(exists($$GOOGLEBENCHMARK_SRCDIR))

!exists($$GOOGLEBENCHMARK_DIR):message("No google benchmark src dir found - set GOOGLEBENCHMARK_DIR to enable.")


INCLUDEPATH *= \
    $$GOOGLEBENCHMARK_SRCDIR \
    $$GOOGLEBENCHMARK_SRCDIR/include

SOURCES += \
    $$GOOGLEBENCHMARK_SRCDIR/src/benchmark.cc
