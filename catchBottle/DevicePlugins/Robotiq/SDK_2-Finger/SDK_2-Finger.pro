#-------------------------------------------------
#
# Created by allen <zxt@aubo-robotics.cn>
# Date: 2017-08-02
#
#-------------------------------------------------

QT       += core gui serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TEMPLATE        = lib
CONFIG         += plugin

INCLUDEPATH += $$PWD \
               $$PWD/ConditionWidget \
               $$PWD/PluginWidgets \
               $$PWD/Src \

DEPENDPATH +=  $$PWD \
               $$PWD/ConditionWidget \
               $$PWD/PluginWidgets \
               $$PWD/Src \

SOURCES += $$PWD/PluginWidgets/robotiq_2fingerpluginwidget.cpp \
    $$PWD/Src/robotiq_2fingerplugin.cpp \
    $$PWD/Src/communicationthread.cpp \
    $$PWD/Src/grippercontrol.cpp \
    $$PWD/Src/dataprocessthread.cpp \
    $$PWD/Src/checksum.cpp \

HEADERS  += $$PWD/PluginWidgets/robotiq_2fingerpluginwidget.h \
    $$PWD/Src/robotiq_2fingerplugin.h \
    $$PWD/Src/communicationthread.h \
    $$PWD/Src/portsettings.h \
    $$PWD/Src/grippersettings.h \
    $$PWD/Src/grippercontrol.h \
    $$PWD/Src/dataprocessthread.h \
    $$PWD/Src/checksum.h \

FORMS    += $$PWD/PluginWidgets/robotiq_2fingerpluginwidget.ui

TARGET  = ../../../Plugins/Robotiq_2FingerPlugin

DISTFILES += $$PWD/Src/robotiq_2-Finger.json

# Common interfaces
include(../../../Commons/commons.pri)

# End of file.
