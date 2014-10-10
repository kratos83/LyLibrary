#-------------------------------------------------
#
# Project created by QtCreator 2013-05-16T20:38:52
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = pdf_read
TEMPLATE = lib
CONFIG += plugin
CONFIG += release

SOURCES += \
    open_pdf.cpp \
    read_pdf.cpp

HEADERS  += \
    open_pdf.h \
    read_pdf.h

FORMS    += \
    read_pdf.ui

unix{
INCLUDEPATH  += /usr/include/poppler/qt4
LIBS         += -lpoppler-qt4
TARGET_EXT = .lux
QMAKE_EXTENSION_SHLIB = lux
DESTDIR = ../lylibrary/data/
}

macx{
INCLUDEPATH  += /opt/local/include/poppler/qt4
LIBS         += -lpoppler-qt4
TARGET_EXT = .lap
QMAKE_EXTENSION_SHLIB = lap
DESTDIR = ../lylibrary/data/
}

windows{
INCLUDEPATH  +=/usr/i686-pc-mingw32/mingw/include/poppler/qt4/
LIBS         += -lpoppler-qt4
TARGET_EXT = .lyb
QMAKE_EXTENSION_SHLIB = lyb
DESTDIR = ../lylibrary/data/
}

RESOURCES += \
    poppler.qrc
