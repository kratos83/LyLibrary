#-------------------------------------------------
#
# Project created by QtCreator 2011-02-27T17:21:20
#
#-------------------------------------------------

QT       += core gui sql xml network qt3support svg

TARGET = ../lylibrary/update
TEMPLATE = app
CONFIG += link_prl
CONFIG += release

OBJECTS_DIR = build
MOC_DIR = build
RCC_DIR = build

SOURCES += main.cpp\
           update.cpp

HEADERS  += update.h

FORMS    += update.ui

RESOURCES += \
    update.qrc

unix{

target.path = /opt/lylibrary

INSTALLS += target

}

