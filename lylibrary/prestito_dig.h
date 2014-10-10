#ifndef PRESTITO_DIG_H
#define PRESTITO_DIG_H

#include <QDialog>
#include <QtSql/QtSql>

namespace Ui {
class prestito_dig;
}

class prestito_dig : public QDialog
{
    Q_OBJECT
    
public:
    explicit prestito_dig(QWidget *parent = 0);
    ~prestito_dig();
    
private slots:
    void lista();
    void list_comp();
    void check_list();
    void clickgrid();
    void aggiorna();
    void elimina();
    void cerca();
    void lista_rietrati();
    void clicca_rientrati();
    void cerca_libri(const QString &);
    void filtro();
    void agg_riavvia_count();
    void el_riavvia_count();

private:
    Ui::prestito_dig *ui;
    QSqlRelationalTableModel *model;
    QLocale *local_settings;

signals:
    void riavvia();
};

#endif // PRESTITO_DIG_H
