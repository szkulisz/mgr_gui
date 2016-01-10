#-------------------------------------------------
#
# Project created by QtCreator 2016-01-09T21:38:05
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = mgr_gui
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    serversettingsdialog.cpp

HEADERS  += mainwindow.h \
    serversettingsdialog.h

FORMS    += mainwindow.ui \
    serversettingsdialog.ui

CONFIG += c++11
