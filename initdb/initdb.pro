greaterThan(QT_MINOR_VERSION, 4){
 QT += core gui sql qt3support
}

greaterThan(QT_MAJOR_VERSION, 4){
 QT += core gui sql widgets printsupport
}
TARGET = initdb-1.0
TEMPLATE = lib
CONFIG += plugin release

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


HEADERS       = connessione.h
SOURCES       = connessione.cpp  
FORMS += connessione.ui
RESOURCES += \
    images.qrc
