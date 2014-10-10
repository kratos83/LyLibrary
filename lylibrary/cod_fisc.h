#ifndef COD_FISC_H
#define COD_FISC_H

#include <QtGui/QDialog>
#include <QDate>

namespace Ui {
    class cod_fisc;
}

class cod_fisc : public QDialog {
    Q_OBJECT
public:
    cod_fisc(QWidget *parent = 0);
    ~cod_fisc();

		void calculateCognome(); // BTT
		void calculateNome(); // FNC
		void calculateData(); // 83S14
		void calculateComune(); // A475
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
    void initComuni();

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

#endif // COD_FISC_H
