#include "inventario_plugin.h"
#include <QtGui>
#include <qglobal.h>

inventario_plugin::inventario_plugin(QObject *parent) :
    QObject(parent)
{
#ifdef Q_WS_WIN
    settingsDir = new QDir(QDir::homePath()+"/lylibrary/");
#else
    settingsDir = new QDir(QDir::homePath()+"/.lylibrary/");
#endif

    if(!settingsDir->exists())
        settingsDir->mkdir(settingsDir->path());

    if(!settingsDir->exists(settingsDir->path()+"/profiles/"))
        settingsDir->mkdir(settingsDir->path()+"/profiles/");

    if(!settingsDir->exists(settingsDir->path()+"/plugins/"))
        settingsDir->mkdir(settingsDir->path()+"/plugins/");

    profDir = new QDir( settingsDir->path()+"/profiles/" );

    general = new QSettings(settingsDir->path()+"/config.conf",QSettings::IniFormat);
}

QWidget* inventario_plugin::creaWidget(){
    return new inventario();
}

QString inventario_plugin::name_menu() const{
    return tr("Plugin");
}
QString inventario_plugin::nameplugin() const{
    return tr("Product inventory");
}

QString inventario_plugin::version() const{
    return tr(general->value("Version/version").toString().replace("'","''").toStdString().c_str());
}

QString inventario_plugin::vendor() const{
    return tr("Codelinsoft");
}

QString inventario_plugin::errorText() const{
    return error;
}

QString inventario_plugin::description() const{
    return tr("Inventario dei prodotti presenti in magazzino");
}

QString inventario_plugin::displayName() const{
    return tr("Plugin Inventario");
}

void inventario_plugin::pluginLoad(){
    creaWidget();
}

void inventario_plugin::pluginUnload(){

    inventario *inv = new inventario;
    inv = 0;
    if(inv)
        delete inv;
}

void inventario_plugin::pluginTranslator(){

    QString locale = general->value("Language/language").toString();
    QTranslator *translator = new QTranslator(this);
    translator->load(":/language/"+locale+".qm");
    qApp->installTranslator(translator);
}

QIcon inventario_plugin::icona(){
    return QIcon(":/images/documentation.png");
}
