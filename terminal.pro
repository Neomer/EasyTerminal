#-------------------------------------------------
#
# Project created by QtCreator 2017-02-04T20:01:32
#
#-------------------------------------------------

QT       += core gui serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = terminal
TEMPLATE = app


SOURCES += main.cpp\
        MainWindow.cpp \
    DeviceDlg.cpp

HEADERS  += MainWindow.h \
    DeviceDlg.h

FORMS    += MainWindow.ui \
    DeviceDlg.ui
