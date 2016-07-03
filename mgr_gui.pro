#-------------------------------------------------
#
# Project created by QtCreator 2016-01-09T21:38:05
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

TARGET = mgr_gui
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    serversettingsdialog.cpp \
    mytcpsocket.cpp \
    qcustomplot-source/qcustomplot.cpp

HEADERS  += mainwindow.h \
    serversettingsdialog.h \
    mytcpsocket.h \
    qcustomplot-source/qcustomplot.h

FORMS    += mainwindow.ui \
    serversettingsdialog.ui

CONFIG += c++11
