#ifndef SETTINGSMANAGER_H
#define SETTINGSMANAGER_H

#include <QObject>
#include <QMap>
#include <QSettings>
#include <QtGui>
#include <QDir>
#include <QVariant>

class SettingsManager: public QObject {
Q_OBJECT

public:
	SettingsManager(QObject *parent=0);
    QVariant generalValue(QString const &key, QVariant const &defaultValue = QVariant()) const;
    void setGeneralValue( QString const &key, QVariant const &value);
    void removeValue(QString const &key);

	QDir *profilesDir() const;

	void createPluginSettings( const QString& );
	void deletePluginSettings( const QString& );
	bool existsPluginSettings( const QString& ) const;
	void setPluginValue( const QString&, const QString&, const QVariant& );
	QVariant pluginValue( const QString&, const QString&, const QVariant& ) const;


	QStringList profilesList( const QString& ) const;
private:
	QMap<QString,QSettings*> pluginSettings;
	QDir *settingsDir, *profDir;
	QSettings *general;
};

extern SettingsManager *settingsManager;

#endif
