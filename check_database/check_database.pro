#-------------------------------------------------
#
# Project created by QtCreator 2013-10-13T16:59:53
#
#-------------------------------------------------

greaterThan(QT_MINOR_VERSION, 4){
 QT += core gui
}

greaterThan(QT_MAJOR_VERSION, 4){
 QT += core gui widgets
}

TARGET = check_database
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
DESTDIR = ../lylibrary/plugin/
}

windows{
TARGET_EXT = .lyb
QMAKE_EXTENSION_SHLIB = lyb
DESTDIR = ../lylibrary/plugin/
}


SOURCES += check_database.cpp \
            checkdatabase_plugin.cpp

HEADERS  += check_database.h \
            checkdatabase_plugin.h

FORMS    += check_database.ui

OTHER_FILES += \
    checkdatabase_plugin.json
