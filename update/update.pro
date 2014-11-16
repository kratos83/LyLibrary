#-------------------------------------------------
#
# Project created by QtCreator 2011-02-27T17:21:20
#
#-------------------------------------------------

QT       += core gui xml network qt3support

TARGET = ../lylibrary/update
TEMPLATE = app
CONFIG += link_prl
CONFIG += release

OBJECTS_DIR = build
MOC_DIR = build
RCC_DIR = build

SOURCES += main.cpp\
           update.cpp \
           settingsmanager.cpp

HEADERS  += update.h \
            settingsmanager.h

FORMS    += update.ui

RESOURCES += \
            update.qrc \
    lang.qrc

unix{

target.path = /opt/codicefiscale

INSTALLS += target
}

TRANSLATIONS = language/English.ts \
               language/Italian.ts
