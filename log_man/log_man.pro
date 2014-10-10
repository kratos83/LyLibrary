#-------------------------------------------------
#
# Project created by QtCreator 2011-09-21T19:12:48
#
#-------------------------------------------------


QT       += core gui

TARGET = log_man-1.0
TEMPLATE = lib
CONFIG += plugin
CONFIG += release

unix{
TARGET_EXT = .lux
QMAKE_EXTENSION_SHLIB = lux
DESTDIR = ../lylibrary/data/
}

macx{
TARGET_EXT = .lap
QMAKE_EXTENSION_SHLIB = lap
DESTDIR = ../lylibrary/data/
}

windows{
TARGET_EXT = .lyb
QMAKE_EXTENSION_SHLIB = lyb
DESTDIR = ../lylibrary/data/
}

SOURCES += license.cpp \
            settingsmanager.cpp

HEADERS  += license.h \
            settingsmanager.h

FORMS    += license.ui

RESOURCES += \
    license.qrc
