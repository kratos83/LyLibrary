#ifndef INTERFACE_PLUGIN_H
#define INTERFACE_PLUGIN_H

#include <QtPlugin>
#include <QMap>

class QIcon;
class QMenu;
class QPushButton;
class QAction;
class QTranslator;

class Interface_plugin {
public:
    virtual ~Interface_plugin() {}
	virtual QString displayName() const = 0;
	virtual void pluginLoad() = 0;
    virtual void pluginUnload() =0;
    virtual QString nameplugin() const = 0;
    virtual QString version() const = 0;
    virtual QString vendor() const = 0;
    virtual QWidget *creaWidget() = 0;
    virtual QString description() const = 0;
    virtual QString errorText() const = 0;
    virtual QIcon icona() = 0;
    virtual QString name_menu() const= 0;
    virtual  void pluginTranslator() = 0;
};

Q_DECLARE_INTERFACE(Interface_plugin, "Luxury_library.Interface_plugin")

#endif
