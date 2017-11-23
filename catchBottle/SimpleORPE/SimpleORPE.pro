#-------------------------------------------------
#
# Project created by QtCreator 2017-07-31T14:19:11
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = SimpleORPE
TEMPLATE = app


SOURCES += main.cpp\
        simpleorpewidget.cpp

HEADERS  += simpleorpewidget.h

FORMS    += simpleorpewidget.ui

include (../Commons/commons.pri)
