#-------------------------------------------------
#
# Project created by QtCreator 2016-01-29T00:12:31
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = HW4
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    gameboard.cpp \
    instr.cpp

HEADERS  += mainwindow.h \
    gameboard.h \
    instr.h

FORMS    += mainwindow.ui \
    gameboard.ui \
    Instr.ui

DISTFILES +=

RESOURCES += \
    resources.qrc

QMAKE_CXXFLAGS = -mmacosx-version-min=10.7 -std=gnu0x -stdlib=libc++

CONFIG += c++11
