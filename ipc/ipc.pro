QT += core gui printsupport network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

RC_FILE = recource.rc

SOURCES += \
    IpcWindow.cpp \
    main.cpp \
    net_data_manager.cpp

HEADERS += \
    IpcWindow.h \
    net_data_manager.h\
    Version.h


LIBS += -L$$OUT_PWD/../libs/ -llibs

FORMS += \
    IpcWindow.ui

