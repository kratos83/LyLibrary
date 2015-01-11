#include "details_plugin.h"
#include "ui_details_plugin.h"

details_plugin::details_plugin(QWidget *parent, QString nome_plugin, QString avvio, QString autore, QString versione,
                               QString descrizione, QString menu, QString n_plugin, QPixmap icona) :
    QDialog(parent),
    ui(new Ui::details_plugin)
{
    ui->setupUi(this);

    ui->nome->setText(nome_plugin);
    ui->avvio->setText(avvio);
    ui->autore->setText(autore);
    ui->versione->setText(versione);
    ui->descrizione->setText(descrizione);
    ui->menu->setText(menu);
    ui->n_plugin_name->setText(n_plugin);
    ui->icona->setPixmap(icona);

    connect(ui->pushButton,SIGNAL(clicked()),this,SLOT(close()));

}

details_plugin::~details_plugin()
{
    delete ui;
}
