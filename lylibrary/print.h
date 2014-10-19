#include <QtSql>
#include <QtGui/QPrinter>
#include <QPainter>
#include <QDialog>
#include <QPrintDialog>
#include <QAbstractPrintDialog>
#include <QLocale>
#include <QModelIndex>
#include "barcode.h"

#ifndef PRINT_H
#define PRINT_H
#define DATA "Data fattura"
#define TIPO_FATTURA "Tipo di Fattura"
#define FATTURA_IMMEDIATA "Fattura immediata"
#define FATTURA_ACCOMMAGNATORIA "Fattura accompagnatoria"
#define FATTURA_PRO_FORMA "Fattura pro forma"
#define DDT "Documento di trasporto"
#define ORDINE "Ordine"
#define PREVENTIVO "Preventivo"
#define PAGAMENTO "Pagamento"
#define NOTE_PAGAMENTO "Note Pagamento"
#define NOTE_FATTURA "Note fattura"
#define COORDINATE_BC "Coordinate Bancarie"
#define AGENTE "Agente"
#define CODICE "Codice"
#define DESCRIZIONE "Descrizione"
#define UM "UM"
#define QUANTITA "Quantità"
#define PREZZO_UNITARIO "Prezzo"
#define SCONTO "Sconto"
#define IMPORTO "Importo"
#define IVA "Iva"
#define SPESE_DI_TRASPORTO "Spese di trasporto"
#define SPESE_DI_INCASSO "Spese di incasso"
#define DETTAGLIO_IVA "C.IVA"
#define IMPONIBILE "Imponibile"
#define IVA_PERC "Aliquota"
#define IMPOSTA "Imposta"
#define TOTALE "Totale"
#define CONTRIBUTO "Contributo conai assolto ove dovuto"
#define SCADENZE "Scadenze"
#define DATA_SC "Data"
#define BANCA "Banca"
#define IBAN "Iban"

class Stampe
{
public:
    Stampe();
    void StampaVoci1(QPrinter *printer,QSqlQueryModel *dataset);
    void creaPagVoci1(int pagNo);
    void stampaarticoli(QPrinter *printer,QSqlQueryModel *dataset);
    void creapagart(int pagNo);
    void stampa_libri(QPrinter *printer,QSqlQueryModel *dataset);
    void creaass(int pagNo);
    void stampaprodotti(QPrinter *printer,QSqlQueryModel *dataset);
    void creapagprodotti(int pagNo);
    void stampa_prodotti_digitali(QPrinter *printer,QSqlQueryModel *dataset);
    void creapagdig(int pagNo);
    void stampabarcode(QPrinter *printer,QSqlQueryModel *dataset,QModelIndex *index, EAN13 *codice);
    void stampabarcodeart(QPrinter *printer, QSqlQueryModel *dataset, QModelIndex *index, EAN13 *codice);
    void stampaforn(QPrinter *printer,QSqlQueryModel *dataset);
    void fornitori(int pagNo);
    void stampafattacq(QPrinter *printer,QSqlQueryModel *dataset);
    void fattacq(int pagNo);
    void stampa_fattura(QPrinter *printer,QSqlRelationalTableModel *dataset);
    void fattura(int pagNo);
    void esporta_pdf(QPrinter *printer,QSqlRelationalTableModel *dataset);
    void pagina(int pagNo);
    void esporta_scarico(QPrinter *printer,QSqlRelationalTableModel *dataset);
    void pagina_scarico(int pagNo);
    void stampa_fatt_vendita_libri(QPrinter *printer, QSqlRelationalTableModel *dataset, QModelIndex index, int id);
    void pagina_fatt_vendita_libri(int pagNo, int id);
    void stampa_fatt_vendita_prod_dig(QPrinter *printer, QSqlRelationalTableModel *dataset, QModelIndex index, int id);
    void pagina_fatt_vendita_prod_dig(int pagNo,int id);
    void intestazione();
    void layout_fattura();
    void print_codice(QPrinter *printer, QImage img, QString cognome, QString nome, bool maschio, bool femmina,
                      QString com_st, QString pr_st, QString codice, QDate date);


    QPrinter *printer;
    QPainter *painter;
    QLocale *local_settings;
    QPrintDialog *stampaDlg;
    barcode *bar;
};

#endif // PRINT_H
