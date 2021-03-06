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
    connect(treeWidget,SIGNAL(itemDoubleClicked(QTreeWidgetItem*,int)),this,SLOT(pluginDoubleClicked(QTreeWidgetItem*,int)));

    //Clicca per abilitare i dettagli dei plugin
    connect(dett_plugin,SIGNAL(clicked()),this,SLOT(add_details()));

    //Chiudi plugin loader
    connect(chiudi,SIGNAL(clicked()),this,SLOT(close()));

    pluginDir = new QDir( QCoreApplication::applicationDirPath()+"/plugin/" );
    loader = new QPluginLoader;

    dett_plugin->setEnabled(false);
}

void pluginmanager::leggiplugin(){

#if defined (Q_OS_LINUX) || defined (Q_OS_UNIX)
QStringList fileNames=pluginDir->entryList( QStringList("*.so"), QDir::Files, QDir::Name);
QStringList toLoad=settingsManager->generalValue( "plugin/loaded",QVariant()).value<QStringList>();
#elif defined Q_OS_WIN
QStringList fileNames=pluginDir->entryList( QStringList("*.dll"), QDir::Files, QDir::Name);
QStringList toLoad=settingsManager->generalValue( "plugin/loaded",QVariant()).value<QStringList>();
#elif defined (Q_OS_MACX)
QStringList fileNames=pluginDir->entryList( QStringList("*.dylib"), QDir::Files, QDir::Name);
QStringList toLoad=settingsManager->generalValue( "plugin/loaded",QVariant()).value<QStringList>();
#endif

    QTreeWidgetItem *item;
    QString cname;

    foreach( QString fileName, fileNames ) {
        loader->setFileName( pluginDir->absoluteFilePath(fileName) );
        QObject *plugin=loader->instance();
        if ( plugin ) {
            cname=QString::fromLatin1( plugin->metaObject()->className() );
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

        dett_plugin->setEnabled(true);
    }
}

void pluginmanager::add_details()
{

    if(!treeWidget->currentItem())
        return;

    loader->setFileName( pluginDir->absoluteFilePath( treeWidget->currentItem()->text(0) ) );
    QObject *plugin=loader->instance();
    if(plugin) {
        Interface_plugin *pi=qobject_cast<Interface_plugin*>( plugin );

        if(!pi)
            return;

        dettagli = new details_plugin(this,pi->nameplugin(),treeWidget->currentItem()->text(1),pi->vendor(),
                                      pi->version(),pi->description(),pi->name_menu(),pi->displayName(),
                                      pi->icona().pixmap(24,24));
        dettagli->exec();

    }
}

void pluginmanager::pluginDoubleClicked(QTreeWidgetItem *item, int column){

  if(column == 1){
    if(!item)
        return;

    loader->setFileName( pluginDir->absoluteFilePath( item->text(0) ) );
    QObject *plugin = loader->instance();
    if(plugin) {
        QString name=plugin->metaObject()->className();

        if( loadedList.contains(name) )
            emit pluginUnload(plugin,item);
        else
            emit pluginLoad(plugin,item);
    }
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
