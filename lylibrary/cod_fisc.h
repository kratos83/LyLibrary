#ifndef COD_FISC_H
#define COD_FISC_H

#include <QtGui>
#include <QDate>
#include <QPainter>

namespace Ui {
    class cod_fisc;
}

class cod_fisc : public QMainWindow {
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
    bool vis_cod_fisc(bool click);
    void stampa_codice();
    void anteprima(QPrinter *printer);

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
        QPixmap img;
        QPainter *painter;

protected:
        void paintEvent(QPaintEvent *event);
};

#endif // COD_FISC_H
