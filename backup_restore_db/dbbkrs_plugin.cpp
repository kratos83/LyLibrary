#include "dbbkrs_plugin.h"

dbbkrs_plugin::dbbkrs_plugin(QObject *parent) :
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

QWidget* dbbkrs_plugin::creaWidget(){
    return new db_bk_rs();
}

QString dbbkrs_plugin::name_menu() const{
    return tr("Plugin");
}
QString dbbkrs_plugin::nameplugin() const{
    return tr("Backup and restore database");
}

QString dbbkrs_plugin::version() const{
    return tr(general->value("Version/version").toString().toStdString().c_str());
}

QString dbbkrs_plugin::vendor() const{
    return tr("Codelinsoft");
}

QString dbbkrs_plugin::errorText() const{
    return error;
}

QString dbbkrs_plugin::description() const{
    return tr("Strumento per il salvataggio e ripristino del database");
}

QString dbbkrs_plugin::displayName() const{
    return tr("Plugin Backup and restore");
}

void dbbkrs_plugin::pluginLoad(){
    creaWidget();
}

void dbbkrs_plugin::pluginUnload(){

    db_bk_rs *db = new db_bk_rs;
    db = 0;
    if(db)
        delete db;
}

QIcon dbbkrs_plugin::icona(){
    return QIcon(":/images/svn-commit.png");
}

Q_EXPORT_PLUGIN2(importdbbkrs_plugin, dbbkrs_plugin)
