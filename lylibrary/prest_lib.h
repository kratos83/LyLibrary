#ifndef PREST_LIB_H
#define PREST_LIB_H

#include <QDialog>
#include <QtGui>
#include <QKeyEvent>
#include <QtSql/QtSql>
#include <QtPrintSupport>
/**
 * @class prest_lib
 * @author Angelo Scarnà <angelo.scarna1@codelinsoft.it>
 * @brief Classe per il prestito dei libri
 *
 * \note Libreria per il prestito dei libri
 * \code
 *  explicit prest_lib(QWidget *parent = 0);
 * \endcode
 *
 * \b codice per la risoluzione degli errori \n
 *
*/
namespace Ui {
    class prest_lib;
}

class prest_lib : public QDialog
{
    Q_OBJECT

public:
    explicit prest_lib(QWidget *parent = 0);
    ~prest_lib();

    /**
         * @brief Evento per la gestione della tastiera.
         * \return QDialog::eventFilter(o,e);
         */
    bool eventFilter(QObject *o, QEvent *e);

private slots:
    /**
         * @brief Evento per la gestione della lista dei libri(lista non rientrati).
         */
    void lista();

    /**
         * @brief Evento per la gestione della lista dei libri(lista completa).
         */
    void list_comp();

    /**
         * @brief Evento per la gestione del checkbox.
         */
    void check_list();

    /**
         * @brief Evento CLICK del ui->tableView.
         */
    void clickgrid();

    /**
         * @brief Evento aggiorna().
         */
    void aggiorna();

    /**
         * @brief Evento elimina().
         */
    void elimina();

    /**
         * @brief Evento cerca().
         */
    void cerca();

    /**
         * @brief Evento lista_rientrati().
         */
    void lista_rietrati();

    /**
         * @brief Evento clicca_rientrati().
         */
    void clicca_rientrati();

    /**
         * @brief Evento cerca_libri(const QString &);
         */
    void cerca_libri(const QString &);

    /**
         * @brief Evento filtro().
         */
    void filtro();

    /**
         * @brief Evento agg_riavvia_count().
         */
    void agg_riavvia_count();

    /**
         * @brief Evento el_riavvia_count().
         */
    void el_riavvia_count();

    /**
         * @brief Evento mousePressEvent(QMouseEvent *).
         */
    void mousePressEvent(QMouseEvent *);

    /**
         * @brief Evento Popup(const QPoint& pt).
         */
    void Popup(const QPoint& pt);

     /**
      * @brief Evento stampaPrestito()
     */
    void stampaPrestito();

    /**
     * @brief Evento anteprima(QPrinter *printer)
    */
   void anteprima(QPrinter *printerr);
private:
    /**
         * @brief Interfaccia client prest_lib::Ui
         */
    Ui::prest_lib *ui;

    /**
         * @brief Classe QSqlRelationalTableModel per la
         * \b gestione delle tabelle.
         */
    QSqlRelationalTableModel *model;

    /**
         * @brief Classe QLocale per la
         * \b gestione della lingua di sistema.
         */
    QLocale *local_settings;

protected:


signals:

    /**
         * @brief Segnale riavvia().
         */
    void riavvia();
};

#endif // PREST_LIB_H
