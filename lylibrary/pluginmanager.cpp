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
    //Chiudi plugin loader
    connect(chiudi,SIGNAL(clicked()),this,SLOT(close()));

    pluginDir = new QDir( QCoreApplication::applicationDirPath()+"/plugin/" );
    loader = new QPluginLoader;
}

void pluginmanager::leggiplugin(){

#if defined (Q_OS_LINUX) || defined (Q_OS_FREEBSD)
QStringList fileNames=pluginDir->entryList( QStringList("*.so"), QDir::Files, QDir::Name);
QStringList toLoad=settingsManager->generalValue( "plugin/loaded",QVariant()).value<QStringList>();
#elif defined Q_OS_WIN
QStringList fileNames=pluginDir->entryList( QStringList("*.dll"), QDir::Files, QDir::Name);
QStringList toLoad=settingsManager->generalValue( "plugin/loaded",QVariant()).value<QStringList>();
#elif defined (Q_OS_MACX)
QStringList fileNames=pluginDir->entryList( QStringList("*.dylib"), QDir::Files, QDir::Name);
QStringList toLoad=settingsManager->generalValue( "plugin/loaded",QVariant()).value<QStringList>();
#endif

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
                    QVector<classe*> labels, labelImage;
                    labelImage.append(new classe(this));
                    labels.append(new classe("Nome: "+fileName+ "\nAvviato..." + "\n"+ "Versione: "+plif->version() + "\n" + "Autore: "+plif->vendor(),this));
                    for(int x = 0; x < labelImage.size(); x++)
                    {
                        labelImage[x]->setPixmap(QPixmap(plif->icona().pixmap(64,64)));
                        labelImage[x]->setFixedSize(QSize(80,80));
                        gridLayout_2->addWidget(labelImage[x]);
                    }
                    for(int i = 0; i < labels.size(); i++){
                        labels[i]->setFont(QFont("Verdana",10));
                        labels[i]->setFrameShape(QFrame::Box);
                        labels[i]->setStyleSheet("QLabel{border: 1px solid white;border-radius: 5px 5px 5px 5px;}QLabel:pressed{background-color: lightbue; color: white; border-radius: 5px 5px 5px 5px;}"
                                                 "QLabel:hover{background-color: lightgrey; color: black; border-radius: 5px 5px 5px 5px;}");
                        labels[i]->setCursor(Qt::PointingHandCursor);
                        connect(labels[i],&classe::clickMouse,this,&pluginmanager::pluginDoubleClicked);
                        if(toLoad.contains(cname)){
                            emit pluginLoad(plugin,fileName);
                            labels[i]->setText("Nome: "+fileName+ "\nAvviato..." + "\n"+ "Versione: "+plif->version() + "\n" + "Autore: "+plif->vendor());
                        }
                        else{
                            emit pluginUnload(plugin,fileName);
                            labels[i]->setText("Nome: "+fileName+ "\nDisattivato..." + "\n"+ "Versione: "+plif->version() + "\n" + "Autore: "+plif->vendor());
                        }
                        gridLayout_1->addWidget(labels.at(i));
                    }
                }
            }
        }
    }
}

void pluginmanager::pluginDoubleClicked(QString plug){

    QStringList m_plug = plug.split("\n");
    QStringList nome_plug = m_plug.at(0).split(" ");
    loader->setFileName( pluginDir->absoluteFilePath( nome_plug.at(1) ) );
    QObject *plugin = loader->instance();
    if(plugin) {
        QString name=plugin->metaObject()->className();
            if( loadedList.contains(name) ){
                emit pluginUnload(plugin,nome_plug.at(1));
                label_testo->setText("Riavviare per rendere effettive le modifiche.");
            }
            else{
                emit pluginLoad(plugin,nome_plug.at(1));
                label_testo->setText("Riavviare per rendere effettive le modifiche.");
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
