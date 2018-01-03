#-------------------------------------------------
#
# Project created by QtCreator 2015-12-22T21:31:40
#
#-------------------------------------------------

QT += core gui widgets
QT += serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ZigBeeATcmd
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h

FORMS    += mainwindow.ui
