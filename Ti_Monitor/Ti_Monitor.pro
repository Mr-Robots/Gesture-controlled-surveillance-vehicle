#-------------------------------------------------
#
# Project created by QtCreator 2015-03-11T17:47:49
#
#-------------------------------------------------

QT       += core gui serialport multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Ti_Monitor
TEMPLATE = app


SOURCES += main.cpp\
        monitorwindow.cpp

HEADERS  += monitorwindow.h

FORMS    += monitorwindow.ui

RESOURCES += \
    Images/Images.qrc

OTHER_FILES += \
    Geo_map.py
