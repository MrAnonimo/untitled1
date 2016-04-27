#-------------------------------------------------
#
# Project created by QtCreator 2016-04-26T08:54:54
#
#-------------------------------------------------

QT       += core gui serialport printsupport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = untitled1
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    arduinoreader.cpp \
    qcustomplot.cpp

HEADERS  += mainwindow.h \
    arduinoreader.h \
    qcustomplot.h

FORMS    += mainwindow.ui
