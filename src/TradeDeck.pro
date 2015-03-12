#-------------------------------------------------
#
# Project created by QtCreator 2015-01-04T22:51:36
#
#-------------------------------------------------

QT       += core gui
QT       += network
QT       += printsupport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = TradeDeck
TEMPLATE = app


SOURCES += main.cpp\
    downloader.cpp \
    mainwindow.cpp \
    qcustomplot.cpp \
    config.cpp \
    marketData.cpp \
    exchangeBot.cpp \
    exchangeBot_btce.cpp \
    order.cpp \
    trade.cpp \
    marketHistory.cpp \
    confirmDialog.cpp \
    settingsDialog.cpp \
    widgetBalances.cpp \
    build-TradeDeck-Desktop_Qt_5_3_MinGW_32bit-Debug/dockwidgettest.cpp

HEADERS  += qcustomplot.h \
    mainwindow.h \
    downloader.h \
    config.h \
    marketData.h \
    exchangeBot.h \
    exchangeBot_btce.h \
    order.h \
    trade.h \
    marketHistory.h \
    confirmDialog.h \
    settingsDialog.h \
    widgetBalances.h \
    build-TradeDeck-Desktop_Qt_5_3_MinGW_32bit-Debug/dockwidgettest.h

FORMS    += mainwindow.ui \
    settingsDialog.ui \
    confirmDialog.ui \
    widgetBalances.ui \
    build-TradeDeck-Desktop_Qt_5_3_MinGW_32bit-Debug/dockwidgettest.ui

OTHER_FILES +=

RESOURCES += \
    TradeDeck.qrc
