#ifndef INTERFACE_PLUGIN_H
#define INTERFACE_PLUGIN_H

#include <QtPlugin>
#include <QMap>

class QIcon;
class QMenu;
class QPushButton;
class QAction;

class Interface_plugin {
public:
	virtual ~Interface_plugin {}
	virtual QString displayName() const = 0;

	virtual QMenu* statusChangeMenu() const = 0;

	virtual void pluginLoad() = 0;
	virtual void pluginUnload() =0;

	virtual QList<QAction*> statusActionList() const = 0;
};

Q_DECLARE_INTERFACE(Interface_plugin, "Luxury_library.Interface_plugin")

#endif
