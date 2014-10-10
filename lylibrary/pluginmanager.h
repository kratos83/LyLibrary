#ifndef PLUGINMANAGER_H
#define PLUGINMANAGER_H

#include <QDialog>
#include <QtGui>
#include <QTreeWidgetItem>
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
    bool readPlugins();


Q_SIGNALS:
    void pluginLoad( QObject*, QTreeWidgetItem* );
    void pluginUnload( QObject*, QTreeWidgetItem* );
    void pluginLoader(const QString&, bool);

public slots:
    void pluginLoaded( const QString&, bool );
    void leggiplugin();

protected slots:
    void readPluginInfo(QTreeWidgetItem*);
    void pluginDoubleClicked(QTreeWidgetItem*);

private:
    QDir *pluginDir;
    QPluginLoader *loader;
    QStringList pluginList;
    QStringList loadedList;
    QList<QPluginLoader*> pluginLoaders;
};

extern pluginmanager *manager;
#endif // PLUGINMANAGER_H
