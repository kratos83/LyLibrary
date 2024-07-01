#ifndef WIDGETPLUGININTERFACE_H
#define WIDGETPLUGININTERFACE_H

#include <QtPlugin>
#include <QString>
#include <QWidget>

class WidgetPluginInterface{
public:
    virtual ~WidgetPluginInterface() {}
    virtual QWidget *creaWidget() = 0;
};

Q_DECLARE_INTERFACE(WidgetPluginInterface,"LyLibrary.WidgetPluginInterface")
#endif // WIDGETPLUGININTERFACE_H
