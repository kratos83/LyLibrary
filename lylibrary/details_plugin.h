#ifndef DETAILS_PLUGIN_H
#define DETAILS_PLUGIN_H

#include <QDialog>
#include <QtGui>
#include <QDebug>

namespace Ui {
class details_plugin;
}

class details_plugin : public QDialog
{
    Q_OBJECT
    
public:
    details_plugin(QWidget *parent, QString nome_plugin, QString avvio,
                   QString autore, QString versione, QString descrizione,
                   QString menu, QString n_plugin, QPixmap icona);
    ~details_plugin();
    
private:
    Ui::details_plugin *ui;
};

#endif // DETAILS_PLUGIN_H
