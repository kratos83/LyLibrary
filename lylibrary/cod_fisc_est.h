#ifndef COD_FISC_EST_H
#define COD_FISC_EST_H

#include <QtGui/QDialog>
#include <QDate>

namespace Ui {
    class cod_fisc;
}

class cod_fisc_est : public QDialog
{
    Q_OBJECT
public:
    explicit cod_fisc_est(QWidget *parent = 0);
    ~cod_fisc_est();
    
    void calculateCognome(); // BTT
    void calculateNome(); // FNC
    void calculateData(); // 83S14
    void calculateStato(); // A475
    void calculateLetteraControllo(); // D
    void merge(); // BTTFNC83S14A475D

    /*
    * -1 spazio
    * 0 consonante
    * 1 vocale
    */
    int checkVocale(QString);

    int getOddCode(QString); // dispari
    int getEvenCode(QString); // pari
    QString mapNumber(int);

    bool chechInput();

private slots:
        void on_esci_clicked();
        void on_pushButtonCalcola_clicked();
        void on_pushButtonCancella_clicked();
        void initStato();

private:
Ui::cod_fisc *ui;
            QDate today;



    QString cognome;
    QString nome;
    bool sesso;
    QString comune;
    int dataGiorno;
    int dataMese;
    int dataAnno;

    QString block1;
    QString block2;
    QString block3;
    QString block4;
    QString block5;

    QString CF;
    
};

#endif // COD_FISC_EST_H
