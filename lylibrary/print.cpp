#include "print.h"
#include <QDir>
#include <qdebug.h>
#include <QPen>
#include <QColor>
#include <QLocale>
#include <QPrintDialog>
#include <QPrintPreviewDialog>
#include <QPainter>
#include <QProcess>
#include <QAbstractPrintDialog>
#include "qstring.h"
#include "locale.h"
#include "settingsmanager.h"

Stampe::Stampe()
{
   // Creazione oggetto printer (puntatore)
   // ed impostazioni varie sul formato pagina, nome file di appoggio
   // formato file prodotto
   // impostazione margine e risoluzione
    printer = new QPrinter();
        // Formato A4 = 210*297 millimetri
        // Formato A4 = 210*297 millimetri

        printer->setPageMargins(QMarginsF(5.0,5.0,5.0,5.0),QPageLayout::Millimeter);
        printer->setPageSize(QPageSize(QPageSize::A4));
        #ifdef Q_OS_LINUX
        printer->setOutputFormat(QPrinter::PdfFormat);
        #endif
        #ifdef Q_OS_MAC
        printer->setOutputFormat(QPrinter::PdfFormat);
        #endif
        printer->setResolution(254);
            // Formato A4 = millimetri 210*297
            // un pollice = 25,4 millimetri
            // risoluzione posta a 254 punti per pollice
            // quindi 10 punti = 1 millimetro
        stampaDlg = new QPrintDialog();
       stampaDlg->setEnabledOptions(QAbstractPrintDialog::PrintPageRange);
        //QVERIFY(stampaDlg->addEnabledOption());
        local_settings = new QLocale(QLocale::system());

           //Creazione oggetto painter che servirà  per disegnare le cose
            painter = new QPainter();

}

void Stampe::stampabarcode(QPrinter *printer, QSqlQueryModel *dataset, QModelIndex *index, EAN13 *codice){


    printer->setResolution(254);
    printer->setPageMargins(QMarginsF(5.0,5.0,5.0,5.0),QPageLayout::Millimeter);
    printer->setPageOrientation(QPageLayout::Portrait);

    // inizio fase di stampa. Si attiva il painter sulla printer
    // impostata in precedenza
  painter->begin(printer);

  painter->setFont(QFont("Arial",8,50));
  painter->setPen(QPen(Qt::black,1));

  if (index->row() < dataset->rowCount())
  {
        // Occorre gestire a mano il controllo di quante righe è possibile stampare
        // per prova si è impostato un massimo di 4 righe di dettaglio per pagina
        // Adeguare il numero di righe in base all'altezza del carattere di stampa
      for (int secRow = 0; secRow < 1; ++secRow)
      {

          QSqlRecord record = dataset->record(index->row());
          if(!record.isEmpty()){
              painter->setPen(QPen(Qt::black,1));

              /************************************************************
               * Prima riga etichetta codice a barre...
               ************************************************************/
              painter->drawText(40,35,400,100,
                                Qt::AlignVCenter | Qt::AlignHCenter,
                           "ISBN - "+record.value("codbarre").toString());
              qreal scale = painter->device()->logicalDpiX() / 25.4;	// dots per mm
              codice->draw(QRectF(50, 100, 37.29 * scale, 25.93 * scale), painter,record.value("codbarre").toString());
              painter->drawText(40,320,400,120,
                                Qt::AlignVCenter | Qt::AlignHCenter,
                                record.value("nome_articolo").toString());

              painter->drawText(490,35,400,100,
                                Qt::AlignVCenter | Qt::AlignHCenter,
                           "ISBN - "+record.value("codbarre").toString());
              codice->draw(QRectF(500, 100, 37.29 * scale, 25.93 * scale), painter,record.value("codbarre").toString());
              painter->drawText(490,320,400,120,
                                Qt::AlignVCenter | Qt::AlignHCenter,
                                record.value("nome_articolo").toString());

              painter->drawText(940,35,400,100,
                                Qt::AlignVCenter | Qt::AlignHCenter,
                           "ISBN - "+record.value("codbarre").toString());
              codice->draw(QRectF(950, 100, 37.29 * scale, 25.93 * scale), painter,record.value("codbarre").toString());
              painter->drawText(940,320,400,120,
                                Qt::AlignVCenter | Qt::AlignHCenter,
                                record.value("nome_articolo").toString());

              painter->drawText(1390,35,400,100,
                                Qt::AlignVCenter | Qt::AlignHCenter,
                           "ISBN - "+record.value("codbarre").toString());
              codice->draw(QRectF(1400, 100, 37.29 * scale, 25.93 * scale), painter,record.value("codbarre").toString());
              painter->drawText(1390,320,400,120,
                                Qt::AlignVCenter | Qt::AlignHCenter,
                                record.value("nome_articolo").toString());


              /************************************************************
               * Seconda riga etichetta codice a barre...
               ************************************************************/
              painter->drawText(40,410,400,100,
                                Qt::AlignVCenter | Qt::AlignHCenter,
                           "ISBN - "+record.value("codbarre").toString());
              codice->draw(QRectF(50, 480, 37.29 * scale, 25.93 * scale), painter,record.value("codbarre").toString());
              painter->drawText(40,700,400,120,
                                Qt::AlignVCenter | Qt::AlignHCenter,
                                record.value("nome_articolo").toString());

              painter->drawText(490,410,400,100,
                                Qt::AlignVCenter | Qt::AlignHCenter,
                           "ISBN - "+record.value("codbarre").toString());
              codice->draw(QRectF(500, 480, 37.29 * scale, 25.93 * scale), painter,record.value("codbarre").toString());
              painter->drawText(490,700,400,120,
                                Qt::AlignVCenter | Qt::AlignHCenter,
                                record.value("nome_articolo").toString());

              painter->drawText(940,410,400,100,
                                Qt::AlignVCenter | Qt::AlignHCenter,
                           "ISBN - "+record.value("codbarre").toString());
              codice->draw(QRectF(950, 480, 37.29 * scale, 25.93 * scale), painter,record.value("codbarre").toString());
              painter->drawText(940,700,400,120,
                                Qt::AlignVCenter | Qt::AlignHCenter,
                                record.value("nome_articolo").toString());

              painter->drawText(1390,410,400,100,
                                Qt::AlignVCenter | Qt::AlignHCenter,
                           "ISBN - "+record.value("codbarre").toString());
              codice->draw(QRectF(1400, 480, 37.29 * scale, 25.93 * scale), painter,record.value("codbarre").toString());
              painter->drawText(1390,700,400,120,
                                Qt::AlignVCenter | Qt::AlignHCenter,
                                record.value("nome_articolo").toString());

              /************************************************************
               * Terza riga etichetta codice a barre...
               ************************************************************/
              painter->drawText(40,785,400,100,
                                Qt::AlignVCenter | Qt::AlignHCenter,
                           "ISBN - "+record.value("codbarre").toString());
              codice->draw(QRectF(50,855 , 37.29 * scale, 25.93 * scale), painter,record.value("codbarre").toString());
              painter->drawText(40,1075,400,120,
                                Qt::AlignVCenter | Qt::AlignHCenter,
                                record.value("nome_articolo").toString());

              painter->drawText(490,785,400,100,
                                Qt::AlignVCenter | Qt::AlignHCenter,
                           "ISBN - "+record.value("codbarre").toString());
              codice->draw(QRectF(500, 855, 37.29 * scale, 25.93 * scale), painter,record.value("codbarre").toString());
              painter->drawText(490,1075,400,120,
                                Qt::AlignVCenter | Qt::AlignHCenter,
                                record.value("nome_articolo").toString());

              painter->drawText(940,785,400,100,
                                Qt::AlignVCenter | Qt::AlignHCenter,
                           "ISBN - "+record.value("codbarre").toString());
              codice->draw(QRectF(950, 855, 37.29 * scale, 25.93 * scale), painter,record.value("codbarre").toString());
              painter->drawText(940,1075,400,120,
                                Qt::AlignVCenter | Qt::AlignHCenter,
                                record.value("nome_articolo").toString());

              painter->drawText(1390,785,400,100,
                                Qt::AlignVCenter | Qt::AlignHCenter,
                           "ISBN - "+record.value("codbarre").toString());
              codice->draw(QRectF(1400, 855, 37.29 * scale, 25.93 * scale), painter,record.value("codbarre").toString());
              painter->drawText(1390,1075,400,120,
                                Qt::AlignVCenter | Qt::AlignHCenter,
                                record.value("nome_articolo").toString());


              /************************************************************
               * Quarta riga etichetta codice a barre...
               ************************************************************/
              painter->drawText(40,1160,400,100,
                                Qt::AlignVCenter | Qt::AlignHCenter,
                           "ISBN - "+record.value("codbarre").toString());
              codice->draw(QRectF(50,1230 , 37.29 * scale, 25.93 * scale), painter,record.value("codbarre").toString());
              painter->drawText(40,1450,400,120,
                                Qt::AlignVCenter | Qt::AlignHCenter,
                                record.value("nome_articolo").toString());

              painter->drawText(490,1160,400,100,
                                Qt::AlignVCenter | Qt::AlignHCenter,
                           "ISBN - "+record.value("codbarre").toString());
              codice->draw(QRectF(500, 1230, 37.29 * scale, 25.93 * scale), painter,record.value("codbarre").toString());
              painter->drawText(490,1450,400,120,
                                Qt::AlignVCenter | Qt::AlignHCenter,
                                record.value("nome_articolo").toString());

              painter->drawText(940,1160,400,100,
                                Qt::AlignVCenter | Qt::AlignHCenter,
                           "ISBN - "+record.value("codbarre").toString());
              codice->draw(QRectF(950, 1230, 37.29 * scale, 25.93 * scale), painter,record.value("codbarre").toString());
              painter->drawText(940,1450,400,120,
                                Qt::AlignVCenter | Qt::AlignHCenter,
                                record.value("nome_articolo").toString());

              painter->drawText(1390,1160,400,100,
                                Qt::AlignVCenter | Qt::AlignHCenter,
                           "ISBN - "+record.value("codbarre").toString());
              codice->draw(QRectF(1400, 1230, 37.29 * scale, 25.93 * scale), painter,record.value("codbarre").toString());
              painter->drawText(1390,1450,400,120,
                                Qt::AlignVCenter | Qt::AlignHCenter,
                                record.value("nome_articolo").toString());

              /************************************************************
               * Quinta riga etichetta codice a barre...
               ************************************************************/
              painter->drawText(40,1535,400,100,
                                Qt::AlignVCenter | Qt::AlignHCenter,
                           "ISBN - "+record.value("codbarre").toString());
              codice->draw(QRectF(50,1605 , 37.29 * scale, 25.93 * scale), painter,record.value("codbarre").toString());
              painter->drawText(40,1825,400,120,
                                Qt::AlignVCenter | Qt::AlignHCenter,
                                record.value("nome_articolo").toString());

              painter->drawText(490,1535,400,100,
                                Qt::AlignVCenter | Qt::AlignHCenter,
                           "ISBN - "+record.value("codbarre").toString());
              codice->draw(QRectF(500, 1605, 37.29 * scale, 25.93 * scale), painter,record.value("codbarre").toString());
              painter->drawText(490,1825,400,120,
                                Qt::AlignVCenter | Qt::AlignHCenter,
                                record.value("nome_articolo").toString());

              painter->drawText(940,1535,400,100,
                                Qt::AlignVCenter | Qt::AlignHCenter,
                           "ISBN - "+record.value("codbarre").toString());
              codice->draw(QRectF(950, 1605, 37.29 * scale, 25.93 * scale), painter,record.value("codbarre").toString());
              painter->drawText(940,1825,400,120,
                                Qt::AlignVCenter | Qt::AlignHCenter,
                                record.value("nome_articolo").toString());

              painter->drawText(1390,1535,400,100,
                                Qt::AlignVCenter | Qt::AlignHCenter,
                           "ISBN - "+record.value("codbarre").toString());
              codice->draw(QRectF(1400, 1605, 37.29 * scale, 25.93 * scale), painter,record.value("codbarre").toString());
              painter->drawText(1390,1825,400,120,
                                Qt::AlignVCenter | Qt::AlignHCenter,
                                record.value("nome_articolo").toString());

              /************************************************************
               * Sesta riga etichetta codice a barre...
               ************************************************************/
              painter->drawText(40,1910,400,100,
                                Qt::AlignVCenter | Qt::AlignHCenter,
                           "ISBN - "+record.value("codbarre").toString());
              codice->draw(QRectF(50,1980 , 37.29 * scale, 25.93 * scale), painter,record.value("codbarre").toString());
              painter->drawText(40,2200,400,120,
                                Qt::AlignVCenter | Qt::AlignHCenter,
                                record.value("nome_articolo").toString());

              painter->drawText(490,1910,400,100,
                                Qt::AlignVCenter | Qt::AlignHCenter,
                           "ISBN - "+record.value("codbarre").toString());
              codice->draw(QRectF(500, 1980, 37.29 * scale, 25.93 * scale), painter,record.value("codbarre").toString());
              painter->drawText(490,2200,400,120,
                                Qt::AlignVCenter | Qt::AlignHCenter,
                                record.value("nome_articolo").toString());

              painter->drawText(940,1910,400,100,
                                Qt::AlignVCenter | Qt::AlignHCenter,
                           "ISBN - "+record.value("codbarre").toString());
              codice->draw(QRectF(950, 1980, 37.29 * scale, 25.93 * scale), painter,record.value("codbarre").toString());
              painter->drawText(940,2200,400,120,
                                Qt::AlignVCenter | Qt::AlignHCenter,
                                record.value("nome_articolo").toString());

              painter->drawText(1390,1910,400,100,
                                Qt::AlignVCenter | Qt::AlignHCenter,
                           "ISBN - "+record.value("codbarre").toString());
              codice->draw(QRectF(1400, 1980, 37.29 * scale, 25.93 * scale), painter,record.value("codbarre").toString());
              painter->drawText(1390,2200,400,120,
                                Qt::AlignVCenter | Qt::AlignHCenter,
                                record.value("nome_articolo").toString());

              /************************************************************
               * Settima riga etichetta codice a barre...
               ************************************************************/
              painter->drawText(40,2285,400,100,
                                Qt::AlignVCenter | Qt::AlignHCenter,
                           "ISBN - "+record.value("codbarre").toString());
              codice->draw(QRectF(50,2355 , 37.29 * scale, 25.93 * scale), painter,record.value("codbarre").toString());
              painter->drawText(40,2575,400,120,
                                Qt::AlignVCenter | Qt::AlignHCenter,
                                record.value("nome_articolo").toString());

              painter->drawText(490,2285,400,100,
                                Qt::AlignVCenter | Qt::AlignHCenter,
                           "ISBN - "+record.value("codbarre").toString());
              codice->draw(QRectF(500, 2355, 37.29 * scale, 25.93 * scale), painter,record.value("codbarre").toString());
              painter->drawText(490,2575,400,120,
                                Qt::AlignVCenter | Qt::AlignHCenter,
                                record.value("nome_articolo").toString());

              painter->drawText(940,2285,400,100,
                                Qt::AlignVCenter | Qt::AlignHCenter,
                           "ISBN - "+record.value("codbarre").toString());
              codice->draw(QRectF(950, 2355, 37.29 * scale, 25.93 * scale), painter,record.value("codbarre").toString());
              painter->drawText(940,2575,400,120,
                                Qt::AlignVCenter | Qt::AlignHCenter,
                                record.value("nome_articolo").toString());

              painter->drawText(1390,2285,400,100,
                                Qt::AlignVCenter | Qt::AlignHCenter,
                           "ISBN - "+record.value("codbarre").toString());
              codice->draw(QRectF(1400, 2355, 37.29 * scale, 25.93 * scale), painter,record.value("codbarre").toString());
              painter->drawText(1390,2575,400,120,
                                Qt::AlignVCenter | Qt::AlignHCenter,
                                record.value("nome_articolo").toString());
            }
        }
      // onde evitare di generare una pagina vuota si controlla a mano
      // se si è reggiunto l'ultimo record
  }
  if(index->row() < dataset->rowCount())
  {
  }
   // chiusura della fase di inserimento dati. Richiamando end()
   // il file viene fisicamente prodotto o mandato in stampa

  painter->end();
}

void Stampe::stampabarcodeart(QPrinter *printer, QSqlQueryModel *dataset, QModelIndex *index, EAN13 *codice){
    printer->setResolution(254);
    printer->setPageMargins(QMarginsF(5.0,5.0,5.0,5.0),QPageLayout::Millimeter);
    printer->setPageOrientation(QPageLayout::Portrait);

    // inizio fase di stampa. Si attiva il painter sulla printer
    // impostata in precedenza
  painter->begin(printer);

  painter->setFont(QFont("Arial",8,50));
  painter->setPen(QPen(Qt::black,1));

  if (index->row() < dataset->rowCount())
  {
        // Occorre gestire a mano il controllo di quante righe è possibile stampare
        // per prova si è impostato un massimo di 4 righe di dettaglio per pagina
        // Adeguare il numero di righe in base all'altezza del carattere di stampa
      for (int secRow = 0; secRow < 1; ++secRow)
      {

          QSqlRecord record = dataset->record(index->row());
          if(!record.isEmpty()){
             // in realt�  il tentativo di controllare se in record è vuoto
             // non funziona nel presente esempio. Non ho capito perché.
              int verticalPos = 50*(secRow+1);
              painter->setPen(QPen(Qt::black,1));
              painter->drawText(140,verticalPos,400,100,
                                Qt::AlignVCenter | Qt::AlignHCenter,
                           "ISBN - "+record.value("codbarre").toString());
              codice->makePattern(record.value("codbarre").toString());
              qreal scale = painter->device()->logicalDpiX() / 25.4;	// dots per mm
              codice->draw(QRectF(150, 150, 37.29 * scale, 25.93 * scale), painter);

              painter->drawText(140,verticalPos,400,800,
                                Qt::AlignVCenter | Qt::AlignHCenter,
                                record.value("nome_articolo").toString());

            }
        }
      // onde evitare di generare una pagina vuota si controlla a mano
      // se si è reggiunto l'ultimo record
  }
  if(index->row() < dataset->rowCount())
  {
  }
   // chiusura della fase di inserimento dati. Richiamando end()
   // il file viene fisicamente prodotto o mandato in stampa

  painter->end();
}

void Stampe::print_codice(QPrinter *printer, QImage img, QString cognome, QString nome,
                         bool maschio, bool femmina, QString com_st, QString pr_st, QString codice, QDate date)
{
        printer->setPageSize(QPageSize(QPageSize::A7));
        printer->setPageOrientation(QPageLayout::Landscape);
        printer->setResolution(254);

           //printer->setOrientation(QPrinter::Landscape);
         // inizio fase di stampa. Si attiva il painter sulla printer
         // impostata in precedenza
      painter->begin(printer);

      painter->drawImage(5,5,img);
      painter->setFont(QFont("Arial",10,50));
      painter->setPen(QPen(Qt::black, 10, Qt::SolidLine, Qt::RoundCap));
      painter->drawText(60,190,480,100,Qt::AlignVCenter | Qt::AlignHCenter | Qt::TextWordWrap,codice);
      painter->drawText(30,240,480,100, Qt::AlignVCenter | Qt::AlignHCenter | Qt::TextWordWrap,cognome);
      painter->drawText(30,290,480,100,Qt::AlignVCenter | Qt::AlignHCenter | Qt::TextWordWrap, nome);
      painter->drawText(40,340,480,100,Qt::AlignVCenter | Qt::AlignHCenter | Qt::TextWordWrap, com_st);
      painter->drawText(40,395,480,100,Qt::AlignVCenter | Qt::AlignHCenter | Qt::TextWordWrap,pr_st);
      painter->drawText(30,450,480,100,Qt::AlignVCenter | Qt::AlignHCenter | Qt::TextWordWrap,date.toString(QString("dd/MM/yyyy")));

      if(maschio){
          painter->drawText(480,335,480,100,Qt::AlignVCenter | Qt::AlignHCenter | Qt::TextWordWrap,"M");
      }
      else if(femmina){
          painter->drawText(480,335,480,100,Qt::AlignVCenter | Qt::AlignHCenter | Qt::TextWordWrap,"F");
      }
      
      painter->setFont(QFont("Arial",12,50));
      painter->drawText(400,440,480,100,Qt::AlignVCenter | Qt::AlignHCenter | Qt::TextWordWrap,"FAC SIMILE");
      painter->end();
}
