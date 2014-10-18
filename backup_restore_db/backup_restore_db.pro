QT       += core widgets gui sql network
TARGET = backup_restore_db
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

HEADERS       = db_bk_rs.h \
    dbbkrs_plugin.h
SOURCES       = db_bk_rs.cpp \  
                dbbkrs_plugin.cpp
FORMS += db_bk_rs.ui
RESOURCES += \
    images.qrc
