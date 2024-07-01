#ifndef USER_H
#define USER_H

#include <QDialog>
#include <QtSql/QtSql>
#include <QPrinter>
#include <QtWidgets>

namespace Ui {
    class user;
}

class user : public QDialog {
    Q_OBJECT
public:
    user(QWidget *parent = 0);
    ~user();
    QString versione;
    bool eventFilter(QObject *o, QEvent *e);

private slots:
    void del_rec();
    void save();
    void lista();
    void clickgrid();
    void cerca();
    void inserisci();
    void on_esci_clicked();
    void nuovo();
    void somma();
    void anteprima();
    void preview_new(QPrinter*);
    void cerca_utente(const QString &);
    void filtro();
    void stampa_pdf();
    void esporta_cvs();
    void mousePressEvent(QMouseEvent *);
    void Popup(const QPoint& pt);
    void clearText();
    void importExcel();

private:
    Ui::user *ui;
    QSqlRelationalTableModel *mod_grid;

signals:
    void clienti_rel();
};

#endif // USER_H

