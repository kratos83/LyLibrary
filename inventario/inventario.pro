#-------------------------------------------------
#
# Project created by QtCreator 2013-04-29T10:02:16
#
#-------------------------------------------------

QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = inventario
TEMPLATE = lib
CONFIG += plugin
CONFIG += release

unix{
TARGET_EXT = .lux
QMAKE_EXTENSION_SHLIB = lux
DESTDIR = ../lylibrary/plugin/
}

macx{
TARGET_EXT = .lap
QMAKE_EXTENSION_SHLIB = lap
DESTDIR = ../lylibrary/data/
}

windows{
TARGET_EXT = .lyb
QMAKE_EXTENSION_SHLIB = lyb
DESTDIR = ../lylibrary/plugin/
}


SOURCES +=\
        inventario.cpp \
    inventario_plugin.cpp \
    export_inv.cpp

HEADERS  += inventario.h \
    inventario_plugin.h \
    export_inv.h

FORMS    += inventario.ui
