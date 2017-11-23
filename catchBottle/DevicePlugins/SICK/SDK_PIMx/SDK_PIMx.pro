#-------------------------------------------------
#
# Created by allen <zxt@aubo-robotics.cn>
# Date: 2017-08-02
#
#-------------------------------------------------

QT       += core gui network webenginewidgets
QT       += webkitwidgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TEMPLATE        = lib
CONFIG         += plugin

INCLUDEPATH += $$PWD \
               $$PWD/Src \
               $$PWD/PluginWidgets \

DEPENDPATH +=  $$PWD \
               $$PWD/Src \
               $$PWD/PluginWidgets \

SOURCES += \ 
    $$PWD/Src/sick_pimxsensorplugin.cpp \
    $$PWD/PluginWidgets/sick_pimxpluginwidget.cpp \
    $$PWD/Src/cameracontrol.cpp \
    $$PWD/Src/processthread.cpp \
    Src/bearingplace.cpp

HEADERS  += \ 
    $$PWD/Src/sick_pimxsensorplugin.h \
    $$PWD/PluginWidgets/sick_pimxpluginwidget.h \
    $$PWD/Src/cameracontrol.h \
    $$PWD/Src/processthread.h \
    $$PWD/Src/sick_cameraconfig.h \
    Src/bearingplace.h

FORMS    += \ 
    $$PWD/PluginWidgets/sick_pimxpluginwidget.ui

TARGET  = ../../../Plugins/SICK_PIMxPlugin

DISTFILES += \
    $$PWD/Src/SICK_PIMx.json

# Common interfaces
include(../../../Commons/commons.pri)

# End of file.

RESOURCES += \
    cameraRec.qrc
