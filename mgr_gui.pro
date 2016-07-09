#-------------------------------------------------
#
# Project created by QtCreator 2016-07-03T01:22:48
#
#-------------------------------------------------

QT       += core gui network opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

TARGET = mgr_gui
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    myglwidget.cpp \
    qcustomplot-source/qcustomplot.cpp

HEADERS  += mainwindow.h \
    myglwidget.h \
    qcustomplot-source/qcustomplot.h

FORMS    += mainwindow.ui

CONFIG += c++11

DISTFILES +=

LIBS += opengl32.lib
