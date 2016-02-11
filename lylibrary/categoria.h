#ifndef PORTO_H
#define PORTO_H

#include <QDialog>
#include <QtGui>
#include <QtSql/QSqlRelationalTableModel>
#include <QKeyEvent>


namespace Ui {
    class porto;
}

class porto : public QDialog
{
    Q_OBJECT

public:
    explicit porto(QWidget *parent = 0);
    ~porto();
    QString versione;
    //Funzione o modulo reiterativo alle
    //funzioni di tastiera
    bool eventFilter(QObject *o, QEvent *e);

    
private:
    Ui::porto *ui;
    QSqlRelationalTableModel *mod_grid;

private slots:
    void salva();
    void aggiorna();
    void elimina();
    void nuovo();
    void cerca();
    void clickgrid();
    void lista();
    void filtro();
    void cerca_libri(const QString &);
    void mousePressEvent(QMouseEvent *event);
    void Popup(const QPoint& pt);
    void clear_lin_text();
};

#endif // PORTO_H
