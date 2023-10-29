QT += core gui sql printsupport quick quickwidgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

RC_FILE = recource.rc

SOURCES += \
    BaseValuesAdderDialog.cpp \
    Constants.cpp \
    DataBaseModule.cpp \
    ManualAdderDialog.cpp \
    MapPointer.cpp \
    MessageReporter.cpp \
    NetDataManager.cpp \
    PlotShow.cpp \
    QCustomPlot.cpp \
    SqlUtility.cpp \
    UselessDataCutter.cpp \
    google_maps_url_maker.cpp \
    main.cpp \
    MainWindow.cpp

HEADERS += \
    BaseValuesAdderDialog.h \
    Constants.h \
    DataBaseModule.h \
    MainWindow.h \
    ManualAdderDialog.h \
    MapPointer.h \
    MessageReporter.h \
    NetDataManager.h \
    PlotShow.h \
    QCustomPlot.h \
    SqlUtility.h \
    Types.h \
    UselessDataCutter.h \
    google_maps_url_maker.h

FORMS += \
    BaseValuesAdderDialog.ui \
    MainWindow.ui \
    ManualAdderDialog.ui

RESOURCES += \
    res.qrc

LIBS += -L$$OUT_PWD/../libs/ -llibs

