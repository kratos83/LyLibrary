#ifndef PLUGINMANAGER_H
#define PLUGINMANAGER_H

#include <QDialog>
#include <QtGui>
#include <QTreeWidgetItem>
#include "details_plugin.h"
#include "ui_pluginmanager.h"

class QDir;
class QPluginLoader;

namespace Ui {
class pluginmanager;
}

class pluginmanager : public QDialog, public Ui::pluginmanager
{
    Q_OBJECT
    
public:
    explicit pluginmanager(QWidget *parent = 0);
    ~pluginmanager();

signals:
    void pluginLoad( QObject*, QTreeWidgetItem* );
    void pluginUnload( QObject*, QTreeWidgetItem* );
    void pluginLoader(const QString&, bool);

public slots:
    void pluginLoaded( const QString&, bool );
    void leggiplugin();
    void add_details();

protected slots:
    void readPluginInfo(QTreeWidgetItem*);
    void pluginDoubleClicked(QTreeWidgetItem*,int column);

private:
    QDir *pluginDir;
    QPluginLoader *loader;
    QStringList pluginList;
    QStringList loadedList;
    QList<QPluginLoader*> pluginLoaders;
    details_plugin *dettagli;
};

extern pluginmanager *manager;
#endif // PLUGINMANAGER_H
