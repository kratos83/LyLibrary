#-------------------------------------------------
#
# Project created by QtCreator 2011-02-27T17:21:20
#
#-------------------------------------------------

QT       += core sql xml network svg multimedia gui qt3support webkit

TARGET = lylibrary
TEMPLATE = app
CONFIG += release

INCLUDEPATH += ../analogclock ../log_man ../initdb

unix{
INCLUDEPATH  += /usr/include/poppler/qt4
LIBS         += -lpoppler-qt4
LIBS += data/liblog_man-1.0.lux
LIBS += data/libinitdb-1.0.lux
LIBS += data/libanalogclock.lux
LIBS += data/libpdf_read.lux
}

macx{
INCLUDEPATH  += /opt/local/include/poppler/qt4
LIBS         += -lpoppler-qt4
LIBS += data/liblog_man-1.0.lap
LIBS += data/libinitdb-1.0.lap
LIBS += data/libanalogclock.lap
LIBS += data/libpdf_read.lap
macx:ICON = images/logo4.icns
}

windows{
INCLUDEPATH  +=C:/Qt/qtcreator-2.0.1/mingw/include/poppler/qt4
LIBS         += -lpoppler-qt4
LIBS += data/log_man-1.0.lyb
LIBS += data/initdb-1.0.lyb
LIBS += data/analogclock.lyb
LIBS += data/pdf_read.lyb
RC_FILE = winicon.rc
}


OBJECTS_DIR = build
MOC_DIR = build
RCC_DIR = build

SOURCES += main.cpp\
        mainwindow.cpp \
        about.cpp \
        print.cpp \
        itdelegato.cpp \
        user.cpp \
        esci.cpp \
        articoli.cpp \
        categoria.cpp \
        presto.cpp \
        topheader.cpp \
        pref.cpp \
        classe.cpp \
        prest_lib.cpp \
        settingsmanager.cpp \
        pluginmanager.cpp \
        barcode.cpp \
        ean13.cpp \
        prodotti_dvd.cpp \
        prestito_app_dig.cpp \
        prestito_dig.cpp \
        progressbar.cpp \
        upgrade.cpp \
        pag.cpp \
        image_resize.cpp \
        imagescene.cpp \
        resizedialog.cpp \
        cod_fisc.cpp \
        causali.cpp \
        iva.cpp \
        azienda.cpp \
        banche.cpp \
        fornitori.cpp \
        fatt_acq.cpp \
        fatt_new.cpp \
        fatt_acq_art.cpp \
        impo_iva_toto.cpp \
        pdf_export.cpp \
        carico_mag.cpp \
        fattura.cpp \
        fattura_rg.cpp \
        fattura_rg_art.cpp \
    scarico_mag.cpp \
    splashscreen.cpp \
    cod_fisc_est.cpp

HEADERS  += mainwindow.h \
            about.h \
            itdelegato.h \
            print.h \
            user.h \
            esci.h \
            articoli.h \
            categoria.h \
            presto.h \
            topheader.h \
            pref.h \
            classe.h \
            prest_lib.h \
            luxuryim.h \
            settingsmanager.h \
            pluginmanager.h \
            barcode.h \
            ean13.h \
            prodotti_dvd.h \
            prestito_app_dig.h \
            prestito_dig.h \
            progressbar.h \
            upgrade.h \
            pag.h \
            image_resize.h \
            imagescene.h \
            resizedialog.h \
            cod_fisc.h \
            causali.h \
            iva.h \
            azienda.h \
            banche.h \
            fornitori.h \
            fatt_acq.h \
            fatt_new.h \
            fatt_acq_art.h \
            impo_iva_toto.h \
    widgetplugininterface.h \
            pdf_export.h \
            carico_mag.h \
            fattura.h \
            fattura_rg.h \
            fattura_rg_art.h \
    scarico_mag.h \
    splashscreen.h \
    cod_fisc_est.h


FORMS    += ui/mainwindow.ui \
            ui/about.ui \
            ui/user.ui \
            ui/esci.ui \
            ui/articoli.ui \
            ui/categoria.ui \
            ui/presto.ui \
            ui/pref.ui \
            ui/prest_lib.ui \
            ui/pluginmanager.ui \
            ui/barcode.ui \
            ui/prodotti_dvd.ui \
            ui/prestito_app_dig.ui \
            ui/prestito_dig.ui \
            ui/upgrade.ui \
            ui/pag.ui \
            ui/cod_fisc.ui \
            ui/causali.ui \
            ui/iva.ui \
            ui/azienda.ui \
            ui/banche.ui \
            ui/fornitori.ui \
            ui/fatt_acq.ui \
            ui/fatt_new.ui \
            ui/fatt_acq_art.ui \
            ui/carico_mag.ui \
            ui/fattura.ui \
            ui/fattura_rg.ui \
            ui/fattura_rg_art.ui \
            ui/scarico_mag.ui

unix{

lylibrary_data.path = /opt/lylibrary/data
lylibrary_data.files = data/*

lylibrary_plugin.path = /opt/lylibrary/plugin
lylibrary_plugin.files= plugin/*

lylibrary_desktop_file.path = /usr/share/applications
lylibrary_desktop_file.files = desktop/*

lylibrary_logo.path=/opt/lylibrary
lylibrary_logo.files= images/logo4.png

target.path = /opt/lylibrary/

INSTALLS += lylibrary_data lylibrary_plugin lylibrary_desktop_file lylibrary_logo target
}
RESOURCES = biblio.qrc

TRANSLATIONS = language/biblio_it.ts \
               language/biblio_en.ts
