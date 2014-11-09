#include "checkdatabase_plugin.h"
#include <qglobal.h>

checkdatabase_plugin::checkdatabase_plugin(QObject *parent) :
    QObject(parent)
{
#ifdef Q_WS_WIN
    settingsDir = new QDir(QDir::homePath()+"/luxury/");
#else
    settingsDir = new QDir(QDir::homePath()+"/.luxury/");
#endif

    if(!settingsDir->exists())
        settingsDir->mkdir(settingsDir->path());

    if(!settingsDir->exists(settingsDir->path()+"/profiles/"))
        settingsDir->mkdir(settingsDir->path()+"/profiles/");

    if(!settingsDir->exists(settingsDir->path()+"/plugins/"))
        settingsDir->mkdir(settingsDir->path()+"/plugins/");

    profDir = new QDir( settingsDir->path()+"/profiles/" );

    general = new QSettings(settingsDir->path()+"/config.conf");
}

QWidget* checkdatabase_plugin::creaWidget(){
    return new check_database();
}

QString checkdatabase_plugin::name_menu() const{
    return tr("Plugin");
}
QString checkdatabase_plugin::nameplugin() const{
    return tr("Correggi database");
}

QString checkdatabase_plugin::version() const{
    return tr(general->value("Version/version").toString().toStdString().c_str());
}

QString checkdatabase_plugin::vendor() const{
    return tr("Codelinsoft");
}

QString checkdatabase_plugin::errorText() const{
    return error;
}

QString checkdatabase_plugin::description() const{
    return tr("Strumento per la correzione degli errori del database");
}

QString checkdatabase_plugin::displayName() const{
    return tr("Plugin Check Database");
}

void checkdatabase_plugin::pluginLoad(){
    creaWidget();
}

void checkdatabase_plugin::pluginUnload(){

    check_database *db = new check_database;
    db = 0;
    if(db)
        delete db;
}

QIcon checkdatabase_plugin::icona(){
    return QIcon(":/images/svn-commit.png");
}

#if QT_VERSION < 0x050000
Q_EXPORT_PLUGIN2(importcheckdatabase_plugin, checkdatabase_plugin)
#endif // QT_VERSION < 0x050000
