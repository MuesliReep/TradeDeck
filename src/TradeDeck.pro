#-------------------------------------------------
#
# Project created by QtCreator 2015-01-04T22:51:36
#
#-------------------------------------------------

QT       += core gui
QT       += network
QT        += printsupport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = TradeDeck
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    Downloader.cpp \
    qcustomplot.cpp

HEADERS  += mainwindow.h \
    Downloader.h \
    qcustomplot.h

FORMS    += mainwindow.ui
