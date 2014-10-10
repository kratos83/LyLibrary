#ifndef INVENTARIO_H
#define INVENTARIO_H

#include <QDialog>
#include <QtGui>

namespace Ui {
class inventario;
}

class inventario : public QDialog
{
    Q_OBJECT
    
public:
    explicit inventario(QWidget *parent = 0);
    ~inventario();
    
private:
    Ui::inventario *ui;
public slots:
    void stampa_inv();
    void esporta_pdf();
    void preview_prev(QPrinter *printer);
    void esporta_prod();
    void st_prod();
    void ant_st_prod(QPrinter *printer);
};

#endif // INVENTARIO_H
