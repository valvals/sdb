QT += core sql

TEMPLATE = lib

CONFIG += staticlib

CONFIG += c++11

DESTDIR = $$OUT_PWD/../libs/

SOURCES += \
    DBJson.cpp

HEADERS += \
    DBJson.h
