#ifndef CHECKDATABASE_PLUGIN_H
#define CHECKDATABASE_PLUGIN_H

#include <QObject>
#include <QWidget>
#include <QDebug>
#include <QSettings>
#include <QTranslator>
#include <QDir>
#include "check_database.h"
#include "../lylibrary/luxuryim.h"

class checkdatabase_plugin : public QObject, public Interface_plugin {
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.importcheckdatabase_plugin" FILE "checkdatabase_plugin.json")
    Q_INTERFACES(Interface_plugin)


public:
    explicit checkdatabase_plugin(QObject *parent = 0);
    QWidget *creaWidget();
    QString nameplugin() const;
        QString version() const;
        QString vendor() const;
        QString errorText() const;
        QString description() const;
        QString displayName() const;
        QIcon icona();
        QString name_menu() const;
        void pluginLoad();
        void pluginUnload();
        void pluginTranslator();
signals:
    
public slots:
        //QIcon seticona();
private:
        QString error;
        QDir *settingsDir, *profDir;
        QSettings *general;
        QIcon icon;
};

#endif // CHECKDATABASE_PLUGIN_H
