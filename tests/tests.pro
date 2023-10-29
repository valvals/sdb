QT += testlib core
TARGET = UnitTests
CONFIG += qt console
CONFIG -= app_bundle
HEADERS += UnitTests.h
SOURCES += UnitTests.cpp
LIBS += -L$$OUT_PWD/../libs/ -llibs


