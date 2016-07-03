#-------------------------------------------------
#
# Project created by QtCreator 2016-07-03T01:22:48
#
#-------------------------------------------------

QT       += core gui network opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = mgr_gui
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    qcustomplot-source/qcustomplot.cpp \
    myglwidget.cpp

HEADERS  += mainwindow.h \
    qcustomplot-source/qcustomplot.h \
    myglwidget.h

FORMS    += mainwindow.ui

CONFIG += c++11
