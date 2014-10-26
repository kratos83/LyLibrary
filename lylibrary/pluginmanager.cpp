#include "pluginmanager.h"
#include "ui_pluginmanager.h"
#include <QPluginLoader>
#include <QDir>

#include <QDebug>
#include "settingsmanager.h"
#include "mainwindow.h"
#include "luxuryim.h"

pluginmanager *manager =0;

pluginmanager::pluginmanager(QWidget *parent) : QDialog(parent)
{
    if(manager)
        delete manager;
    manager = this;

    setupUi(this);
    //Cambia riga
    connect(treeWidget,SIGNAL(currentItemChanged(QTreeWidgetItem*,QTreeWidgetItem*)),this,SLOT(readPluginInfo(QTreeWidgetItem*)));
    //Clicca due volte per abilitare o disabilitare un plugin
    connect(treeWidget,SIGNAL(itemDoubleClicked(QTreeWidgetItem*,int)),this,SLOT(pluginDoubleClicked(QTreeWidgetItem*)));

    connect(treeWidget,SIGNAL(itemClicked(QTreeWidgetItem*,int)),this,SLOT(leggiplugin()));

    //Chiudi plugin loader
    connect(chiudi,SIGNAL(clicked()),this,SLOT(close()));

    pluginDir = new QDir( QCoreApplication::applicationDirPath()+"/plugin/" );
    loader = new QPluginLoader;

}

bool pluginmanager::readPlugins(){
    #if defined Q_OS_LINUX
    QStringList fileNames=pluginDir->entryList( QStringList("*.lux"), QDir::Files, QDir::Name);
    QStringList toLoad=settingsManager->generalValue( "plugin/loaded",QVariant()).value<QStringList>();
    #elif defined Q_OS_WIN
    QStringList fileNames=pluginDir->entryList( QStringList("*.lyb"), QDir::Files, QDir::Name);
    QStringList toLoad=settingsManager->generalValue( "plugin/loaded",QVariant()).value<QStringList>();
    #elif defined Q_OS_MACX
    QStringList fileNames=pluginDir->entryList( QStringList("*.lap"), QDir::Files, QDir::Name);
    QStringList toLoad=settingsManager->generalValue( "plugin/loaded",QVariant()).value<QStringList>();
    #endif
    QTreeWidgetItem *item;
    QString cname;



    foreach( QString fileName, fileNames ) {
        loader->setFileName( pluginDir->absoluteFilePath(fileName) );
        QObject *plugin=loader->instance();
        if ( plugin ) {
            cname=QString::fromAscii( plugin->metaObject()->className() );
            if ( !pluginList.contains( cname ) ) {
                Interface_plugin *plif=qobject_cast<Interface_plugin*>( plugin );
                if(plif) {
                    pluginList.append( cname );
                    QStringList values;
                    values<< fileName << "" << plif->version() << plif->vendor();
                    item=new QTreeWidgetItem( treeWidget, values );
                    if(toLoad.contains(cname)){
                          emit pluginLoad( plugin, item );
                    }
                    else
                          emit pluginUnload( plugin, item );
                }
            }
        }
    }

    return true;
}

void pluginmanager::leggiplugin(){

    #if defined Q_OS_LINUX
    QStringList fileNames=pluginDir->entryList( QStringList("*.lux"), QDir::Files, QDir::Name);
    QStringList toLoad=settingsManager->generalValue( "plugin/loaded",QVariant()).value<QStringList>();
    #elif defined Q_OS_WIN
    QStringList fileNames=pluginDir->entryList( QStringList("*.lyb"), QDir::Files, QDir::Name);
    QStringList toLoad=settingsManager->generalValue( "plugin/loaded",QVariant()).value<QStringList>();
    #elif defined Q_OS_MACX
    QStringList fileNames=pluginDir->entryList( QStringList("*.lap"), QDir::Files, QDir::Name);
    QStringList toLoad=settingsManager->generalValue( "plugin/loaded",QVariant()).value<QStringList>();
    #endif

    QTreeWidgetItem *item;
    QString cname;

    foreach( QString fileName, fileNames ) {
        loader->setFileName( pluginDir->absoluteFilePath(fileName) );
        QObject *plugin=loader->instance();
        if ( plugin ) {
            cname=QString::fromAscii( plugin->metaObject()->className() );
            if ( !pluginList.contains( cname ) ) {
                Interface_plugin *plif=qobject_cast<Interface_plugin*>( plugin );
                if(plif) {
                    pluginList.append( cname );
                    pluginLoaders.append(loader);
                    QStringList values;
                    values<< fileName << "" << plif->version() << plif->vendor();
                    item=new QTreeWidgetItem( treeWidget, values );
                    if(toLoad.contains(cname)){
                          item->setText( 1, tr("Avviato...") );
                          QString ic = ":/images/dialog-ok-apply.png";
                          QIcon icona(ic);
                          item->setIcon(1,icona);
                              emit pluginLoad( plugin, item );
                    }
                    else{
                        item->setText( 1, tr("Disattivato...") );
                        QString ic = ":/images/list-remove.png";
                        QIcon icona(ic);
                        item->setIcon(1,icona);
                          emit pluginUnload( plugin, item );
                    }
                }
            }
        }
    }
}

void pluginmanager::readPluginInfo(QTreeWidgetItem *item){

    if(!item)
        return;

    loader->setFileName( pluginDir->absoluteFilePath( item->text(0) ) );
    QObject *plugin=loader->instance();
    if(plugin) {
        Interface_plugin *pi=qobject_cast<Interface_plugin*>( plugin );

        if(!pi)
            return;

        nome->setText(pi->nameplugin());
        autore->setText(pi->vendor());
        versione->setText(pi->version());
        descrizione->setText(pi->description());
       //FIXME
       //if( !loadedList.contains( plugin->metaObject()->className() ) )
            //delete plugin;
    }
}

void pluginmanager::pluginDoubleClicked(QTreeWidgetItem *item){

    if(!item)
        return;

    loader->setFileName( pluginDir->absoluteFilePath( item->text(0) ) );
    QObject *plugin = loader->instance();
    if(plugin) {
        QString name=plugin->metaObject()->className();

        if( loadedList.contains(name) )
            Q_EMIT pluginUnload(plugin,item);
        else
            Q_EMIT pluginLoad(plugin,item);
    }
}

void pluginmanager::pluginLoaded(const QString &plugin, bool loaded){

    if( loaded )
        loadedList.append( plugin );
    else
       loadedList.removeAll( plugin );

    settingsManager->setGeneralValue("plugin/loaded",loadedList);
}

pluginmanager::~pluginmanager(){
}
