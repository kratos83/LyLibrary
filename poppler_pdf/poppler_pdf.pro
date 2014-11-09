#-------------------------------------------------
#
# Project created by QtCreator 2013-05-16T20:38:52
#
#-------------------------------------------------

greaterThan(QT_MINOR_VERSION, 4){
 QT += core gui qt3support

unix{
INCLUDEPATH  += /usr/include/poppler/qt4
LIBS         += -lpoppler-qt4
}

windows{
INCLUDEPATH  +=C:/Qt/qtcreator-2.0.1/mingw/include/poppler/qt4
LIBS         += -lpoppler-qt4
}
}

greaterThan(QT_MAJOR_VERSION, 4){
 QT += core gui widgets printsupport
unix{
INCLUDEPATH  += /usr/include/poppler/qt5
LIBS         += -lpoppler-qt5
}

windows{
INCLUDEPATH  +=C:/Qt/qtcreator-2.0.1/mingw/include/poppler/qt4
LIBS         += -lpoppler-qt4
}
}

TARGET = pdf_read
TEMPLATE = lib
CONFIG += plugin release

SOURCES += \
    open_pdf.cpp \
    read_pdf.cpp

HEADERS  += \
    open_pdf.h \
    read_pdf.h

FORMS    += \
    read_pdf.ui

unix{
TARGET_EXT = .lux
QMAKE_EXTENSION_SHLIB = lux
DESTDIR = ../lylibrary/data/
}

windows{
TARGET_EXT = .lyb
QMAKE_EXTENSION_SHLIB = lyb
DESTDIR = ../lylibrary/data/
}

RESOURCES += \
    poppler.qrc
