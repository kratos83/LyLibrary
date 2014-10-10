#include "pdf_export.h"
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

pdf_export::pdf_export()
{
    // Creazione oggetto printer (puntatore)
    // ed impostazioni varie sul formato pagina, nome file di appoggio
    // formato file prodotto
    // impostazione margine e risoluzione
    printer = new QPrinter();
         // Formato A4 = 210*297 millimetri
         // Formato A4 = 210*297 millimetri

         printer->setPageMargins(5.0,5.0,5.0,5.0,QPrinter::Millimeter);
         printer->setPaperSize(QPrinter::A4);
         #ifdef Q_OS_WIN
         printer->setOutputFormat(QPrinter::PdfFormat);
         #endif
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

            //Creazione oggetto painter che servirÃ  per disegnare le cose
             painter = new QPainter();

}

void pdf_export::StampaVoci1(QPrinter *printer, QSqlQueryModel *dataset){

     // la funzione stampaVoci accoglie come parametro un oggetto puntatore
     // che rappresenta il dataset da mandare in stampa e che quindi
     // contiene i dati del report

   printer->setPageMargins(5.0,5.0,5.0,5.0,QPrinter::Millimeter);
    printer->setPaperSize(QPrinter::A4);
    printer->setResolution(254);

       //printer->setOrientation(QPrinter::Landscape);
     // inizio fase di stampa. Si attiva il painter sulla printer
     // impostata in precedenza
  painter->begin(printer);

   int pagNo = 1;

   // si richiama la sotto funzione che "disegna" il modulo di stampa
   this->creaPagVoci1(pagNo);
     ++pagNo;

   int recNo = 0;

   // per ogni record del data set si effettua la stampa del dettaglio

   while (recNo < dataset->rowCount())
   {
        // printer->setOrientation(QPrinter::Landscape);
         // Occorre gestire a mano il controllo di quante righe Ã¨ possibile stampare
         // per prova si Ã¨ impostato un massimo di 4 righe di dettaglio per pagina
         // Adeguare il numero di righe in base all'altezza del carattere di stampa

       for (int secRow = 0; secRow < 30; ++secRow)
       {
           QSqlRecord record = dataset->record(recNo);

           if (!record.isEmpty())
           {
              // in realtÃ  il tentativo di controllare se in record Ã¨ vuoto
              // non funziona nel presente esempio. Non ho capito perchÃ©.

               int verticalPos = 200+100*(secRow+1);

               painter->setPen(QPen(Qt::black,1));

               painter->drawText(0,verticalPos,200,100,
                            Qt::AlignVCenter | Qt::AlignHCenter,
                            record.value("id").toString());
               painter->drawText(200,verticalPos,300,100,
                            Qt::AlignVCenter|Qt::AlignHCenter,
                            record.value("nome").toString());
               painter->drawText(400,verticalPos,600,100,
                            Qt::AlignVCenter|Qt::AlignHCenter,
                            record.value("cognome").toString());
               painter->drawText(600,verticalPos,800,100,
                                 Qt::AlignVCenter|Qt::AlignHCenter,
                                 record.value("indirizzo").toString());
               painter->drawText(850,verticalPos,1000,100,
                                 Qt::AlignVCenter|Qt::AlignHCenter,
                                 record.value("telefono").toString());
               painter->drawText(1050,verticalPos,1200,100,
                                 Qt::AlignVCenter|Qt::AlignHCenter,
                                 record.value("email").toString());
               painter->drawText(1300,verticalPos,1400,100,
                                 Qt::AlignVCenter|Qt::AlignHCenter,
                                 record.value("cod_fisc").toString());
               painter->drawText(1550,verticalPos,1600,100,
                                 Qt::AlignVCenter|Qt::AlignHCenter,
                                 record.value("part_iva").toString());
               painter->drawText(1750,verticalPos,1800,100,
                                 Qt::AlignVCenter|Qt::AlignHCenter,
                                 record.value("fax").toString());
               ++recNo;
           }

       }

       // onde evitare di generare una pagina vuota si controlla a mano
       // se si Ã¨ reggiunto l'ultimo record

       if(recNo < dataset->rowCount())
       {
           printer->newPage();
           this->creaPagVoci1(pagNo);
           ++pagNo;
       }
    }

    // chiusura della fase di inserimento dati. Richiamando end()
    // il file viene fisicamente prodotto o mandato in stampa
   painter->end();

}

void pdf_export::creaPagVoci1(int pagNo){
    // La funzione disegna gli oggetti "Statici" presenti su ogni pagina
    // di fatto prepara il modulo di stampa per accogliere poi i dati

  painter->setFont(QFont("Arial",12,75));
  painter->setPen(QPen(Qt::blue,1));
  painter->drawRect(0,0,2870,2000);

  painter->setPen(QPen(Qt::black,1));
  painter->drawRect(0,0,2870,120);
  painter->drawText(0,0,2870,120,
                   Qt::AlignVCenter|Qt::AlignHCenter,
                   "Elenco Clienti");

  //Valorizzazione header box intestazione colonne

  painter->drawText(0,200,200,100,
                   Qt::AlignVCenter|Qt::AlignHCenter,
                   "Codice");
  painter->drawText(200,200,300,100,
                   Qt::AlignVCenter|Qt::AlignHCenter,
                   "Nome");
  painter->drawText(400,200,600,100,
                   Qt::AlignVCenter|Qt::AlignHCenter,
                   "Cognome");
  painter->drawText(600,200,800,100,
                    Qt::AlignVCenter|Qt::AlignHCenter,
                    "Indirizzo");
  painter->drawText(850,200,1000,100,
                    Qt::AlignVCenter|Qt::AlignHCenter,
                    "Telefono");
  painter->drawText(1050,200,1200,100,
                    Qt::AlignVCenter|Qt::AlignHCenter,
                    "E-mail");
  painter->drawText(1300,200,1400,100,
                    Qt::AlignVCenter|Qt::AlignHCenter,
                    "Codice Fiscale");
  painter->drawText(1550,200,1600,100,
                    Qt::AlignVCenter|Qt::AlignHCenter,
                    "Partita Iva");
  painter->drawText(1750,200,1800,100,
                    Qt::AlignVCenter|Qt::AlignHCenter,
                    "Fax");

  painter->setFont(QFont("Arial",10,50));
  painter->drawText(0,1900,2770,100,
                       Qt::AlignVCenter|Qt::AlignRight,
                       "Pagina: "+QString("%1").arg(pagNo)+"   ");
  painter->drawText(0,1900,2770,100,
                       Qt::AlignVCenter|Qt::AlignLeft,
                    "Lylibrary - versione "+settingsManager->generalValue("Version/version",QVariant()).toString());
  painter->drawLine(0,1900,2870,1900);
}

void pdf_export::stampa_banca(QPrinter *printer, QSqlRelationalTableModel *dataset){
    // la funzione stampa_fattura accoglie come parametro un oggetto puntatore
    // che rappresenta il dataset da mandare in stampa e che quindi
    // contiene i dati del report

    printer->setResolution(254);
    printer->setPageMargins(5.0,5.0,5.0,5.0,QPrinter::Millimeter);
    printer->setOrientation(QPrinter::Landscape);

    // inizio fase di stampa. Si attiva il painter sulla printer
    // impostata in precedenza
  painter->begin(printer);

   int pagNo = 1;
  this->banca(pagNo);
  ++pagNo;
  // si richiama la sotto funzione che "disegna" il modulo di stampa



  int recNo = 0;
  painter->setFont(QFont("Arial",8,50));
          painter->setPen(QPen(Qt::black,1));

  // per ogni record del data set si effettua la stampa del dettaglio

  while (recNo < dataset->rowCount())
  {
        //printer->setOrientation(QPrinter::Landscape);
        // Occorre gestire a mano il controllo di quante righe Ã¨ possibile stampare
        // per prova si Ã¨ impostato un massimo di 4 righe di dettaglio per pagina
        // Adeguare il numero di righe in base all'altezza del carattere di stampa

      for (int secRow = 0; secRow < 20; ++secRow)
      {
          QSqlRecord record = dataset->record(recNo);

          if (!record.isEmpty())
          {
             // in realtÃ  il tentativo di controllare se in record Ã¨ vuoto
             // non funziona nel presente esempio. Non ho capito perchÃ©.

              int verticalPos = 150+60*(secRow+1);
              painter->setPen(QPen(Qt::black,1));
              painter->drawText(0,verticalPos,200,100,
                                Qt::AlignVCenter | Qt::AlignHCenter,
                           record.value("id").toString());
              painter->drawText(100,verticalPos,1000,100,
                                Qt::AlignVCenter|Qt::AlignHCenter,
                           record.value("nome_banca").toString());
              painter->drawText(550,verticalPos,1000,100,
                                Qt::AlignVCenter|Qt::AlignHCenter,
                                record.value("iban").toString());
              painter->drawText(1100,verticalPos,1200,100,
                                Qt::AlignVCenter|Qt::AlignHCenter,
                                record.value("indirizzo").toString());
              painter->drawText(1500,verticalPos,1200,100,
                                Qt::AlignVCenter|Qt::AlignHCenter,
                                record.value("cap").toString());
              painter->drawText(1900,verticalPos,1200,100,
                                Qt::AlignVCenter|Qt::AlignHCenter,
                                record.value("comune").toString());
              ++recNo;
          }

      }

      // onde evitare di generare una pagina vuota si controlla a mano
      // se si Ã¨ reggiunto l'ultimo record

      if(recNo < dataset->rowCount())
      {
          printer->newPage();
          this->banca(pagNo);
          ++pagNo;
      }
   }

   // chiusura della fase di inserimento dati. Richiamando end()
   // il file viene fisicamente prodotto o mandato in stampa

  painter->end();
}

void pdf_export::banca(int pagNo){

    // La funzione disegna gli oggetti "Statici" presenti su ogni pagina
    // di fatto prepara il modulo di stampa per accogliere poi i dati

  painter->setFont(QFont("Arial",12,75));
  painter->setPen(QPen(Qt::blue,1));
  painter->drawRect(0,0,2870,2000);

  painter->drawRect(0,0,2870,120);
  painter->drawText(0,0,2870,120,
                   Qt::AlignVCenter|Qt::AlignHCenter,
                   "Elenco banche");


  //Valorizzazione header box intestazione colonne

  painter->drawText(0,100,200,100,
                    Qt::AlignVCenter | Qt::AlignHCenter,
                    "ID");
  painter->drawText(100,100,1000,100,
                    Qt::AlignVCenter|Qt::AlignHCenter,
                    "Banca");
  painter->drawText(550,100,1000,100,
                    Qt::AlignVCenter|Qt::AlignHCenter,
                    "IBAN");
  painter->drawText(1100,100,1200,100,
                    Qt::AlignVCenter|Qt::AlignHCenter,
                    "Indirizzo");
  painter->drawText(1500,100,1200,100,
                    Qt::AlignVCenter|Qt::AlignHCenter,
                    "Cap");
  painter->drawText(1900,100,1200,100,
                    Qt::AlignVCenter|Qt::AlignHCenter,
                    "Comune");


      painter->setFont(QFont("Arial",10,50));
      painter->drawText(0,1900,2700,100,
                       Qt::AlignVCenter|Qt::AlignRight,
                       "Pagina: "+QString("%1").arg(pagNo)+"   ");
      painter->drawText(20,1900,2700,100,
                       Qt::AlignVCenter|Qt::AlignLeft,
                       "Lylibrary - versione "+settingsManager->generalValue("Version/version",QVariant()).toString());
      painter->drawLine(0,1900,2870,1900);
}
