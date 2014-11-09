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

        printer->setPageMargins(5.0,5.0,5.0,5.0,QPrinter::Millimeter);
        printer->setPaperSize(QPrinter::A4);
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

           //Creazione oggetto painter che servir�  per disegnare le cose
            painter = new QPainter();

}

void Stampe::StampaVoci1(QPrinter *printer, QSqlQueryModel *dataset){

     // la funzione stampaVoci accoglie come parametro un oggetto puntatore
     // che rappresenta il dataset da mandare in stampa e che quindi
     // contiene i dati del report

    printer->setPageMargins(5.0,5.0,5.0,5.0,QPrinter::Millimeter);
     printer->setPaperSize(QPrinter::A4);
     printer->setOrientation(QPrinter::Landscape);
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
          // Occorre gestire a mano il controllo di quante righe è possibile stampare
          // per prova si è impostato un massimo di 4 righe di dettaglio per pagina
          // Adeguare il numero di righe in base all'altezza del carattere di stampa

        for (int secRow = 0; secRow < 30; ++secRow)
        {
            QSqlRecord record = dataset->record(recNo);

            if (!record.isEmpty())
            {
               // in realt�  il tentativo di controllare se in record è vuoto
               // non funziona nel presente esempio. Non ho capito perché.

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
        // se si è reggiunto l'ultimo record

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

void Stampe::creaPagVoci1(int pagNo){
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

void Stampe::stampaarticoli(QPrinter *printer,QSqlQueryModel *dataset){

    // la funzione stampaarticoli accoglie come parametro un oggetto puntatore
    // che rappresenta il dataset da mandare in stampa e che quindi
    // contiene i dati del report
    printer->setResolution(254);
     printer->setPageMargins(5.0,5.0,5.0,5.0,QPrinter::Millimeter);
      printer->setOrientation(QPrinter::Landscape);
    // inizio fase di stampa. Si attiva il painter sulla printer
    // impostata in precedenza
  painter->begin(printer);



  // si richiama la sotto funzione che "disegna" il modulo di stampa
  int pagNo = 0;
  ++pagNo;
  this->creapagart(pagNo);


  int recNo = 0;

  // per ogni record del data set si effettua la stampa del dettaglio

  while (recNo < dataset->rowCount())
  {

        // Occorre gestire a mano il controllo di quante righe è possibile stampare
        // per prova si è impostato un massimo di 4 righe di dettaglio per pagina
        // Adeguare il numero di righe in base all'altezza del carattere di stampa

      for (int secRow = 0; secRow < 16; ++secRow)
      {
          QSqlRecord record = dataset->record(recNo);

          if (!record.isEmpty())
          {
             // in realt�  il tentativo di controllare se in record è vuoto
             // non funziona nel presente esempio. Non ho capito perché.

              int verticalPos = 200+100*(secRow+1);

              painter->setPen(QPen(Qt::black,1));
              painter->drawText(0,verticalPos,200,100,
                                Qt::AlignVCenter | Qt::AlignHCenter,
                           record.value("cod_articolo").toString());
              painter->drawText(200,verticalPos,400,100,
                                Qt::AlignVCenter|Qt::AlignHCenter,
                           record.value("codbarre").toString());
              painter->drawText(600,verticalPos,600,100,
                           Qt::AlignVCenter|Qt::AlignHCenter,
                           record.value("nome_articolo").toString());
              painter->drawText(1000,verticalPos,800,100,
                                Qt::AlignVCenter|Qt::AlignHCenter,
                                record.value("lingua").toString());
              painter->drawText(1400,verticalPos,1000,100,
                                Qt::AlignVCenter|Qt::AlignHCenter,
                                record.value("categ").toString());
              painter->drawText(1600,verticalPos,1400,100,
                                Qt::AlignVCenter|Qt::AlignHCenter,
                                record.value("scaffale").toString());
              painter->drawText(1800,verticalPos,1600,100,
                                Qt::AlignVCenter|Qt::AlignHCenter,
                                record.value("quantita").toString());
              ++recNo;
          }

      }

      // onde evitare di generare una pagina vuota si controlla a mano
      // se si è reggiunto l'ultimo record

      if(recNo < dataset->rowCount())
      {
          printer->newPage();
          this->creapagart(pagNo);
          ++pagNo;
      }
   }

   // chiusura della fase di inserimento dati. Richiamando end()
   // il file viene fisicamente prodotto o mandato in stampa

  painter->end();
}

void Stampe::creapagart(int pagNo){

    // La funzione disegna gli oggetti "Statici" presenti su ogni pagina
    // di fatto prepara il modulo di stampa per accogliere poi i dati

  painter->setFont(QFont("Arial",12,75));
  painter->setPen(QPen(Qt::blue,1));
  painter->drawRect(0,0,2870,2000);

  painter->setPen(QPen(Qt::blue,1));
  painter->drawRect(0,0,2870,120);
  painter->drawText(0,0,2870,120,
                   Qt::AlignVCenter|Qt::AlignHCenter,
                   "Elenco libri");

  //Valorizzazione header box intestazione colonne

  painter->drawText(0,200,200,100,
                   Qt::AlignVCenter|Qt::AlignHCenter,
                   "ID");
  painter->drawText(200,200,400,100,
                   Qt::AlignVCenter|Qt::AlignHCenter,
                   "Barcode");
  painter->drawText(600,200,600,100,
                   Qt::AlignVCenter|Qt::AlignHCenter,
                   "Titolo");
  painter->drawText(1000,200,800,100,
                    Qt::AlignVCenter|Qt::AlignHCenter,
                    "Lingua");
  painter->drawText(1400,200,1000,100,
                    Qt::AlignVCenter|Qt::AlignHCenter,
                    "Categoria");
  painter->drawText(1600,200,1400,100,
                    Qt::AlignVCenter|Qt::AlignHCenter,
                    "Collocazione");
  painter->drawText(1800,200,1600,100,
                    Qt::AlignVCenter|Qt::AlignHCenter,
                    "Quantit�");

      painter->setFont(QFont("Arial",10,50));
      painter->drawText(0,1900,2700,100,
                       Qt::AlignVCenter|Qt::AlignRight,
                       "Pagina: "+QString("%1").arg(pagNo)+"   ");
      painter->drawText(0,1900,2700,100,
                       Qt::AlignVCenter|Qt::AlignLeft,
                       "Lylibrary - versione "+settingsManager->generalValue("Version/version",QVariant()).toString());
      painter->drawLine(0,1900,2870,1900);
}

void Stampe::stampa_libri(QPrinter *printer,QSqlQueryModel *dataset){

    // la funzione stampaarticoli accoglie come parametro un oggetto puntatore
    // che rappresenta il dataset da mandare in stampa e che quindi
    // contiene i dati del report

    printer->setResolution(254);
    printer->setPageMargins(5.0,5.0,5.0,5.0,QPrinter::Millimeter);
    printer->setOrientation(QPrinter::Landscape);

    // inizio fase di stampa. Si attiva il painter sulla printer
    // impostata in precedenza
  painter->begin(printer);

   int pagNo = 1;
  this->creaass(pagNo);
  ++pagNo;
  // si richiama la sotto funzione che "disegna" il modulo di stampa



  int recNo = 0;
  painter->setFont(QFont("Arial",8,50));
          painter->setPen(QPen(Qt::black,1));

  // per ogni record del data set si effettua la stampa del dettaglio

  while (recNo < dataset->rowCount())
  {
        //printer->setOrientation(QPrinter::Landscape);
        // Occorre gestire a mano il controllo di quante righe è possibile stampare
        // per prova si è impostato un massimo di 4 righe di dettaglio per pagina
        // Adeguare il numero di righe in base all'altezza del carattere di stampa

      for (int secRow = 0; secRow < 20; ++secRow)
      {
          QSqlRecord record = dataset->record(recNo);

          if (!record.isEmpty())
          {
             // in realt�  il tentativo di controllare se in record è vuoto
             // non funziona nel presente esempio. Non ho capito perché.

              int verticalPos = 150+60*(secRow+1);
              painter->setPen(QPen(Qt::black,1));
              painter->drawText(0,verticalPos,200,100,
                                Qt::AlignVCenter | Qt::AlignHCenter,
                           record.value("id").toString());
              painter->drawText(200,verticalPos,1000,100,
                                Qt::AlignVCenter|Qt::AlignHCenter,
                           record.value("cliente").toString());
              painter->drawText(900,verticalPos,1000,100,
                                Qt::AlignVCenter|Qt::AlignHCenter,
                                record.value("libro").toString());
              painter->drawText(1500,verticalPos,1200,100,
                                Qt::AlignVCenter|Qt::AlignHCenter,
                                record.value("data_prestito").toString());
              painter->drawText(1900,verticalPos,1400,100,
                                Qt::AlignVCenter|Qt::AlignHCenter,
                                record.value("data_rientro").toString());
              ++recNo;
          }

      }

      // onde evitare di generare una pagina vuota si controlla a mano
      // se si è reggiunto l'ultimo record

      if(recNo < dataset->rowCount())
      {
          printer->newPage();
          this->creaass(pagNo);
          ++pagNo;
      }
   }

   // chiusura della fase di inserimento dati. Richiamando end()
   // il file viene fisicamente prodotto o mandato in stampa

  painter->end();
}

void Stampe::creaass(int pagNo){

    // La funzione disegna gli oggetti "Statici" presenti su ogni pagina
    // di fatto prepara il modulo di stampa per accogliere poi i dati

  painter->setFont(QFont("Arial",12,75));
  painter->setPen(QPen(Qt::blue,1));
  painter->drawRect(0,0,2870,2000);

  painter->drawRect(0,0,2870,120);
  painter->drawText(0,0,2870,120,
                   Qt::AlignVCenter|Qt::AlignHCenter,
                   "Elenco prestiti libri");
  

  //Valorizzazione header box intestazione colonne

  painter->drawText(0,100,200,100,
                   Qt::AlignVCenter|Qt::AlignHCenter,
                   "ID");
  painter->drawText(250,100,800,100,
                   Qt::AlignVCenter|Qt::AlignHCenter,
                   "Cliente");
  painter->drawText(850,100,1100,100,
                    Qt::AlignVCenter|Qt::AlignHCenter,
                    "Libri");
  painter->drawText(1500,100,1200,100,
                    Qt::AlignVCenter|Qt::AlignHCenter,
                    "Data prestito");
  painter->drawText(1900,100,1400,100,
                    Qt::AlignVCenter|Qt::AlignHCenter,
                    "Data rientro");

      painter->setFont(QFont("Arial",10,50));
      painter->drawText(0,1900,2700,100,
                       Qt::AlignVCenter|Qt::AlignRight,
                       "Pagina: "+QString("%1").arg(pagNo)+"   ");
      painter->drawText(0,1900,2700,100,
                       Qt::AlignVCenter|Qt::AlignLeft,
                       "Lylibrary - versione "+settingsManager->generalValue("Version/version",QVariant()).toString());
      painter->drawLine(0,1900,2870,1900);
}

void Stampe::stampaprodotti(QPrinter *printer,QSqlQueryModel *dataset){

    // la funzione stampaarticoli accoglie come parametro un oggetto puntatore
    // che rappresenta il dataset da mandare in stampa e che quindi
    // contiene i dati del report
    printer->setResolution(254);
     printer->setPageMargins(5.0,5.0,5.0,5.0,QPrinter::Millimeter);
      printer->setOrientation(QPrinter::Landscape);
    // inizio fase di stampa. Si attiva il painter sulla printer
    // impostata in precedenza
  painter->begin(printer);



  // si richiama la sotto funzione che "disegna" il modulo di stampa
  int pagNo = 0;
  ++pagNo;
  this->creapagprodotti(pagNo);


  int recNo = 0;

  // per ogni record del data set si effettua la stampa del dettaglio

  while (recNo < dataset->rowCount())
  {

        // Occorre gestire a mano il controllo di quante righe è possibile stampare
        // per prova si è impostato un massimo di 4 righe di dettaglio per pagina
        // Adeguare il numero di righe in base all'altezza del carattere di stampa

      for (int secRow = 0; secRow < 16; ++secRow)
      {
          QSqlRecord record = dataset->record(recNo);

          if (!record.isEmpty())
          {
             // in realt�  il tentativo di controllare se in record è vuoto
             // non funziona nel presente esempio. Non ho capito perché.

              int verticalPos = 200+100*(secRow+1);

              painter->setPen(QPen(Qt::black,1));
              painter->drawText(0,verticalPos,200,100,
                                Qt::AlignVCenter | Qt::AlignHCenter,
                           record.value("cod_prodotto").toString());
              painter->drawText(200,verticalPos,400,100,
                                Qt::AlignVCenter|Qt::AlignHCenter,
                           record.value("codbarre").toString());
              painter->drawText(600,verticalPos,600,100,
                           Qt::AlignVCenter|Qt::AlignHCenter,
                           record.value("nome_prodotto").toString());
              painter->drawText(1000,verticalPos,800,100,
                                Qt::AlignVCenter|Qt::AlignHCenter,
                                record.value("lingua").toString());
              painter->drawText(1400,verticalPos,1000,100,
                                Qt::AlignVCenter|Qt::AlignHCenter,
                                record.value("categ").toString());
              painter->drawText(1600,verticalPos,1400,100,
                                Qt::AlignVCenter|Qt::AlignHCenter,
                                record.value("scaffale").toString());
              painter->drawText(1800,verticalPos,1600,100,
                                Qt::AlignVCenter|Qt::AlignHCenter,
                                record.value("quantita").toString());
              ++recNo;
          }

      }

      // onde evitare di generare una pagina vuota si controlla a mano
      // se si è reggiunto l'ultimo record

      if(recNo < dataset->rowCount())
      {
          printer->newPage();
          this->creapagprodotti(pagNo);
          ++pagNo;
      }
   }

   // chiusura della fase di inserimento dati. Richiamando end()
   // il file viene fisicamente prodotto o mandato in stampa

  painter->end();
}

void Stampe::creapagprodotti(int pagNo){

    // La funzione disegna gli oggetti "Statici" presenti su ogni pagina
    // di fatto prepara il modulo di stampa per accogliere poi i dati

  painter->setFont(QFont("Arial",12,75));
  painter->setPen(QPen(Qt::blue,1));
  painter->drawRect(0,0,2870,2000);

  painter->setPen(QPen(Qt::blue,1));
  painter->drawRect(0,0,2870,120);
  painter->drawText(0,0,2870,120,
                   Qt::AlignVCenter|Qt::AlignHCenter,
                   "Elenco prodotti digitali");

  //Valorizzazione header box intestazione colonne

  painter->drawText(0,200,200,100,
                   Qt::AlignVCenter|Qt::AlignHCenter,
                   "ID");
  painter->drawText(200,200,400,100,
                   Qt::AlignVCenter|Qt::AlignHCenter,
                   "Barcode");
  painter->drawText(600,200,600,100,
                   Qt::AlignVCenter|Qt::AlignHCenter,
                   "Prodotto");
  painter->drawText(1000,200,800,100,
                    Qt::AlignVCenter|Qt::AlignHCenter,
                    "Lingua");
  painter->drawText(1400,200,1000,100,
                    Qt::AlignVCenter|Qt::AlignHCenter,
                    "Categoria");
  painter->drawText(1600,200,1400,100,
                    Qt::AlignVCenter|Qt::AlignHCenter,
                    "Scaffale");
  painter->drawText(1800,200,1600,100,
                    Qt::AlignVCenter|Qt::AlignHCenter,
                    "Quantit�");

      painter->setFont(QFont("Arial",10,50));
      painter->drawText(0,1900,2700,100,
                       Qt::AlignVCenter|Qt::AlignRight,
                       "Pagina: "+QString("%1").arg(pagNo)+"   ");
      painter->drawText(0,1900,2700,100,
                       Qt::AlignVCenter|Qt::AlignLeft,
                       "Lylibrary - versione "+settingsManager->generalValue("Version/version",QVariant()).toString());
      painter->drawLine(0,1900,2870,1900);
}

void Stampe::stampa_prodotti_digitali(QPrinter *printer,QSqlQueryModel *dataset){

    // la funzione stampaarticoli accoglie come parametro un oggetto puntatore
    // che rappresenta il dataset da mandare in stampa e che quindi
    // contiene i dati del report

    printer->setResolution(254);
    printer->setPageMargins(5.0,5.0,5.0,5.0,QPrinter::Millimeter);
    printer->setOrientation(QPrinter::Landscape);

    // inizio fase di stampa. Si attiva il painter sulla printer
    // impostata in precedenza
  painter->begin(printer);

   int pagNo = 1;
  this->creapagdig(pagNo);
  ++pagNo;
  // si richiama la sotto funzione che "disegna" il modulo di stampa



  int recNo = 0;
  painter->setFont(QFont("Arial",8,50));
          painter->setPen(QPen(Qt::black,1));

  // per ogni record del data set si effettua la stampa del dettaglio

  while (recNo < dataset->rowCount())
  {
        //printer->setOrientation(QPrinter::Landscape);
        // Occorre gestire a mano il controllo di quante righe è possibile stampare
        // per prova si è impostato un massimo di 4 righe di dettaglio per pagina
        // Adeguare il numero di righe in base all'altezza del carattere di stampa

      for (int secRow = 0; secRow < 20; ++secRow)
      {
          QSqlRecord record = dataset->record(recNo);

          if (!record.isEmpty())
          {
             // in realt�  il tentativo di controllare se in record è vuoto
             // non funziona nel presente esempio. Non ho capito perché.

              int verticalPos = 150+60*(secRow+1);
              painter->setPen(QPen(Qt::black,1));
              painter->drawText(0,verticalPos,200,100,
                                Qt::AlignVCenter | Qt::AlignHCenter,
                           record.value("id").toString());
              painter->drawText(200,verticalPos,1000,100,
                                Qt::AlignVCenter|Qt::AlignHCenter,
                           record.value("cliente").toString());
              painter->drawText(900,verticalPos,1000,100,
                                Qt::AlignVCenter|Qt::AlignHCenter,
                                record.value("dvd").toString());
              painter->drawText(1500,verticalPos,1200,100,
                                Qt::AlignVCenter|Qt::AlignHCenter,
                                record.value("data_prestito").toString());
              painter->drawText(1900,verticalPos,1400,100,
                                Qt::AlignVCenter|Qt::AlignHCenter,
                                record.value("data_rientro").toString());
              ++recNo;
          }

      }

      // onde evitare di generare una pagina vuota si controlla a mano
      // se si è reggiunto l'ultimo record

      if(recNo < dataset->rowCount())
      {
          printer->newPage();
          this->creapagdig(pagNo);
          ++pagNo;
      }
   }

   // chiusura della fase di inserimento dati. Richiamando end()
   // il file viene fisicamente prodotto o mandato in stampa

  painter->end();
}

void Stampe::creapagdig(int pagNo){

    // La funzione disegna gli oggetti "Statici" presenti su ogni pagina
    // di fatto prepara il modulo di stampa per accogliere poi i dati

  painter->setFont(QFont("Arial",12,75));
  painter->setPen(QPen(Qt::blue,1));
  painter->drawRect(0,0,2870,2000);

  painter->drawRect(0,0,2870,120);
  painter->drawText(0,0,2870,120,
                   Qt::AlignVCenter|Qt::AlignHCenter,
                   "Elenco prodotti digitali prestati");


  //Valorizzazione header box intestazione colonne

  painter->drawText(0,100,200,100,
                   Qt::AlignVCenter|Qt::AlignHCenter,
                   "ID");
  painter->drawText(250,100,800,100,
                   Qt::AlignVCenter|Qt::AlignHCenter,
                   "Cliente");
  painter->drawText(850,100,1100,100,
                    Qt::AlignVCenter|Qt::AlignHCenter,
                    "Prodotti digitali");
  painter->drawText(1500,100,1200,100,
                    Qt::AlignVCenter|Qt::AlignHCenter,
                    "Data prestito");
  painter->drawText(1900,100,1400,100,
                    Qt::AlignVCenter|Qt::AlignHCenter,
                    "Data rientro");

      painter->setFont(QFont("Arial",10,50));
      painter->drawText(0,1900,2700,100,
                       Qt::AlignVCenter|Qt::AlignRight,
                       "Pagina: "+QString("%1").arg(pagNo)+"   ");
      painter->drawText(0,1900,2700,100,
                       Qt::AlignVCenter|Qt::AlignLeft,
                       "Lylibrary - versione "+settingsManager->generalValue("Version/version",QVariant()).toString());
      painter->drawLine(0,1900,2870,1900);
}

void Stampe::stampabarcode(QPrinter *printer, QSqlQueryModel *dataset, QModelIndex *index, EAN13 *codice){


    printer->setResolution(254);
    printer->setPageMargins(5.0,5.0,5.0,5.0,QPrinter::Millimeter);
    printer->setOrientation(QPrinter::Portrait);

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
    printer->setPageMargins(5.0,5.0,5.0,5.0,QPrinter::Millimeter);
    printer->setOrientation(QPrinter::Portrait);

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

void Stampe::stampaforn(QPrinter *printer, QSqlQueryModel *dataset){

    // la funzione st_rma accoglie come parametro un oggetto puntatore
    // che rappresenta il dataset da mandare in stampa e che quindi
    // contiene i dati del report
    printer->setResolution(254);
    printer->setPageMargins(5.0,5.0,5.0,5.0,QPrinter::Millimeter);
    printer->setPaperSize(QPrinter::A4);
    printer->setOrientation(QPrinter::Landscape);

    // inizio fase di stampa. Si attiva il painter sulla printer
    // impostata in precedenza
  painter->begin(printer);

    int pagNo = 1;
    fornitori(pagNo);
   ++pagNo;
   // si richiama la sotto funzione che "disegna" il modulo di stampa



   int recNo = 0;
   painter->setFont(QFont("Arial",8,50));
           painter->setPen(QPen(Qt::black,1));

   // per ogni record del data set si effettua la stampa del dettaglio

   while (recNo < dataset->rowCount())
   {
         //printer->setOrientation(QPrinter::Landscape);
         // Occorre gestire a mano il controllo di quante righe �? possibile stampare
         // per prova si �? impostato un massimo di 4 righe di dettaglio per pagina
         // Adeguare il numero di righe in base all'altezza del carattere di stampa

       for (int secRow = 0; secRow < 16; ++secRow)
       {
           QSqlRecord record = dataset->record(recNo);

           if (!record.isEmpty())
           {
                int verticalPos = 200+100*(secRow+1);

                painter->setPen(QPen(Qt::black,2));
                painter->drawText(0,verticalPos,200,100,
                                  Qt::AlignHCenter | Qt::AlignVCenter,
                                  record.value("id").toString());
                painter->drawText(150,verticalPos,300,100,
                                 Qt::AlignHCenter | Qt::AlignVCenter,
                                  record.value("ragione_sociale").toString());
                painter->drawText(400,verticalPos,500,100,
                                  Qt::AlignHCenter | Qt::AlignVCenter,
                                  record.value("part_iva").toString());
                painter->drawText(600,verticalPos,650,100,
                                  Qt::AlignHCenter | Qt::AlignVCenter,
                                  record.value("cod_fiscale").toString());
                painter->drawText(800,verticalPos,800,100,
                                  Qt::AlignHCenter | Qt::AlignVCenter,
                                  record.value("indirizzo").toString());
                painter->drawText(900,verticalPos,950,100,
                                  Qt::AlignHCenter | Qt::AlignVCenter,
                                  record.value("cap").toString());
                painter->drawText(1000,verticalPos,1100,100,
                                  Qt::AlignHCenter | Qt::AlignVCenter,
                                  record.value("localita").toString());
                painter->drawText(1100,verticalPos,1250,100,
                                  Qt::AlignHCenter | Qt::AlignVCenter,
                                  "("+record.value("prov").toString()+")");
                painter->drawText(1200,verticalPos,1400,100,
                                  Qt::AlignHCenter | Qt::AlignVCenter,
                                  record.value("telefono").toString());
                painter->drawText(1300,verticalPos,1550,100,
                                  Qt::AlignHCenter | Qt::AlignVCenter,
                                  record.value("fax").toString());
                painter->drawText(1500,verticalPos,1700,100,
                                  Qt::AlignHCenter | Qt::AlignVCenter,
                                  record.value("email").toString());
                painter->drawText(1800,verticalPos,1850,100,
                                  Qt::AlignHCenter | Qt::AlignVCenter,
                                  record.value("sito").toString());
                ++recNo;

           }
       }
       if(recNo < dataset->rowCount())
       {
           printer->newPage();
           fornitori(pagNo);
           ++pagNo;
       }
   }
   painter->end();
}

void Stampe::fornitori(int pagNo){

    painter->setFont(QFont("Arial",12,75));
    painter->setPen(QPen(Qt::blue,2));
    painter->drawRect(0,0,2870,2000);
    painter->drawRect(0,0,2870,120);
    painter->drawText(0,0,2870,120,
                      Qt::AlignVCenter | Qt::AlignHCenter,
                      "Elenco fornitori");

    //Valorizzazione header box intestazione colonne
    painter->setPen(QPen(Qt::black,2));
    painter->drawText(0,200,200,100,
                      Qt::AlignHCenter | Qt::AlignVCenter,
                      "ID");
    painter->drawText(150,200,350,100,
                     Qt::AlignHCenter | Qt::AlignVCenter,
                      "Ragione sociale");
    painter->drawText(400,200,500,100,
                      Qt::AlignHCenter | Qt::AlignVCenter,
                      "Partita iva");
    painter->drawText(600,200,650,100,
                      Qt::AlignHCenter | Qt::AlignVCenter,
                      "Codice fiscale");
    painter->drawText(800,200,800,100,
                      Qt::AlignHCenter | Qt::AlignVCenter,
                      "Indirizzo");
    painter->drawText(900,200,950,100,
                      Qt::AlignHCenter | Qt::AlignVCenter,
                      "CAP");
    painter->drawText(1000,200,1100,100,
                      Qt::AlignHCenter | Qt::AlignVCenter,
                      "Localit�");
    painter->drawText(1100,200,1250,100,
                      Qt::AlignHCenter | Qt::AlignVCenter,
                      "PROV");
    painter->drawText(1200,200,1400,100,
                      Qt::AlignHCenter | Qt::AlignVCenter,
                      "Telefono");
    painter->drawText(1300,200,1550,100,
                      Qt::AlignHCenter | Qt::AlignVCenter,
                      "Fax");
    painter->drawText(1500,200,1700,100,
                      Qt::AlignHCenter | Qt::AlignVCenter,
                      "E-mail");
    painter->drawText(1800,200,1850,100,
                      Qt::AlignHCenter | Qt::AlignVCenter,
                      "Sito web");

    painter->setFont(QFont("Arial",10,50));
    painter->setPen(QPen(Qt::blue,1));
    painter->drawText(0,1900,2700,100,
                      Qt::AlignVCenter | Qt::AlignRight,
                      "Pagina: "+QString("%1").arg(pagNo)+" ");
    painter->drawText(0,1900,2700,100,
                      Qt::AlignVCenter | Qt::AlignLeft,
                      "Fabaria Gest - verione 1.0");
    painter->drawLine(0,1900,2870,1900);
}

void Stampe::stampafattacq(QPrinter *printer, QSqlQueryModel *dataset){

    // la funzione stampaarticoli accoglie come parametro un oggetto puntatore
    // che rappresenta il dataset da mandare in stampa e che quindi
    // contiene i dati del report

    printer->setResolution(254);
    printer->setPageMargins(5.0,5.0,5.0,5.0,QPrinter::Millimeter);
    printer->setOrientation(QPrinter::Landscape);

    // inizio fase di stampa. Si attiva il painter sulla printer
    // impostata in precedenza
  painter->begin(printer);

   int pagNo = 1;
  this->fattacq(pagNo);
  ++pagNo;
  // si richiama la sotto funzione che "disegna" il modulo di stampa



  int recNo = 0;
  painter->setFont(QFont("Arial",8,50));
          painter->setPen(QPen(Qt::black,1));

  // per ogni record del data set si effettua la stampa del dettaglio

  while (recNo < dataset->rowCount())
  {
        //printer->setOrientation(QPrinter::Landscape);
        // Occorre gestire a mano il controllo di quante righe è possibile stampare
        // per prova si è impostato un massimo di 4 righe di dettaglio per pagina
        // Adeguare il numero di righe in base all'altezza del carattere di stampa

      for (int secRow = 0; secRow < 20; ++secRow)
      {
          QSqlRecord record = dataset->record(recNo);

          if (!record.isEmpty())
          {
             // in realt�  il tentativo di controllare se in record è vuoto
             // non funziona nel presente esempio. Non ho capito perché.

              int verticalPos = 150+60*(secRow+1);
              painter->setPen(QPen(Qt::black,1));
              painter->drawText(0,verticalPos,200,100,
                                Qt::AlignVCenter | Qt::AlignHCenter,
                           record.value("id").toString());
              painter->drawText(200,verticalPos,1000,100,
                                Qt::AlignVCenter|Qt::AlignHCenter,
                           record.value("data").toString());
              painter->drawText(900,verticalPos,1000,100,
                                Qt::AlignVCenter|Qt::AlignHCenter,
                                record.value("fornitore").toString());
              painter->drawText(1500,verticalPos,1200,100,
                                Qt::AlignVCenter|Qt::AlignHCenter,
                                record.value("totale").toString());
              ++recNo;
          }

      }

      // onde evitare di generare una pagina vuota si controlla a mano
      // se si è reggiunto l'ultimo record

      if(recNo < dataset->rowCount())
      {
          printer->newPage();
          this->fattacq(pagNo);
          ++pagNo;
      }
   }

   // chiusura della fase di inserimento dati. Richiamando end()
   // il file viene fisicamente prodotto o mandato in stampa

  painter->end();
}

void Stampe::fattacq(int pagNo){

    // La funzione disegna gli oggetti "Statici" presenti su ogni pagina
    // di fatto prepara il modulo di stampa per accogliere poi i dati

  painter->setFont(QFont("Arial",12,75));
  painter->setPen(QPen(Qt::blue,1));
  painter->drawRect(0,0,2870,2000);

  painter->drawRect(0,0,2870,120);
  painter->drawText(0,0,2870,120,
                   Qt::AlignVCenter|Qt::AlignHCenter,
                   "Elenco Fatture d'acquisto");


  //Valorizzazione header box intestazione colonne

  painter->drawText(0,100,200,100,
                   Qt::AlignVCenter|Qt::AlignHCenter,
                   "ID");
  painter->drawText(250,100,800,100,
                   Qt::AlignVCenter|Qt::AlignHCenter,
                   "Data");
  painter->drawText(850,100,1100,100,
                    Qt::AlignVCenter|Qt::AlignHCenter,
                    "Fornitore");
  painter->drawText(1500,100,1200,100,
                    Qt::AlignVCenter|Qt::AlignHCenter,
                    "Totale");


      painter->setFont(QFont("Arial",10,50));
      painter->drawText(0,1900,2700,100,
                       Qt::AlignVCenter|Qt::AlignRight,
                       "Pagina: "+QString("%1").arg(pagNo)+"   ");
      painter->drawText(20,1900,2700,100,
                       Qt::AlignVCenter|Qt::AlignLeft,
                       "Lylibrary - versione "+settingsManager->generalValue("Version/version",QVariant()).toString());
      painter->drawLine(0,1900,2870,1900);
}

void Stampe::stampa_fattura(QPrinter *printer, QSqlRelationalTableModel *dataset){
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
  this->fattura(pagNo);
  ++pagNo;
  // si richiama la sotto funzione che "disegna" il modulo di stampa



  int recNo = 0;
  painter->setFont(QFont("Arial",8,50));
          painter->setPen(QPen(Qt::black,1));

  // per ogni record del data set si effettua la stampa del dettaglio

  while (recNo < dataset->rowCount())
  {
        //printer->setOrientation(QPrinter::Landscape);
        // Occorre gestire a mano il controllo di quante righe è possibile stampare
        // per prova si è impostato un massimo di 4 righe di dettaglio per pagina
        // Adeguare il numero di righe in base all'altezza del carattere di stampa

      for (int secRow = 0; secRow < 20; ++secRow)
      {
          QSqlRecord record = dataset->record(recNo);

          if (!record.isEmpty())
          {
             // in realt�  il tentativo di controllare se in record è vuoto
             // non funziona nel presente esempio. Non ho capito perché.

              int verticalPos = 150+60*(secRow+1);
              painter->setPen(QPen(Qt::black,1));
              painter->drawText(0,verticalPos,200,100,
                                Qt::AlignVCenter | Qt::AlignHCenter,
                           record.value("id").toString());
              painter->drawText(200,verticalPos,1000,100,
                                Qt::AlignVCenter|Qt::AlignHCenter,
                           record.value("data").toString());
              painter->drawText(900,verticalPos,1000,100,
                                Qt::AlignVCenter|Qt::AlignHCenter,
                                record.value("fornitore").toString());
              painter->drawText(1500,verticalPos,1200,100,
                                Qt::AlignVCenter|Qt::AlignHCenter,
                                record.value("totale").toString());
              ++recNo;
          }

      }

      // onde evitare di generare una pagina vuota si controlla a mano
      // se si è reggiunto l'ultimo record

      if(recNo < dataset->rowCount())
      {
          printer->newPage();
          this->fattura(pagNo);
          ++pagNo;
      }
   }

   // chiusura della fase di inserimento dati. Richiamando end()
   // il file viene fisicamente prodotto o mandato in stampa

  painter->end();
}

void Stampe::fattura(int pagNo){

    // La funzione disegna gli oggetti "Statici" presenti su ogni pagina
    // di fatto prepara il modulo di stampa per accogliere poi i dati

  painter->setFont(QFont("Arial",12,75));
  painter->setPen(QPen(Qt::blue,1));
  painter->drawRect(0,0,2870,2000);

  painter->drawRect(0,0,2870,120);
  painter->drawText(0,0,2870,120,
                   Qt::AlignVCenter|Qt::AlignHCenter,
                   "Elenco Fatture di vendita");


  //Valorizzazione header box intestazione colonne

  painter->drawText(0,100,200,100,
                   Qt::AlignVCenter|Qt::AlignHCenter,
                   "ID");
  painter->drawText(250,100,800,100,
                   Qt::AlignVCenter|Qt::AlignHCenter,
                   "Data");
  painter->drawText(850,100,1100,100,
                    Qt::AlignVCenter|Qt::AlignHCenter,
                    "Fornitore o cliente");
  painter->drawText(1500,100,1200,100,
                    Qt::AlignVCenter|Qt::AlignHCenter,
                    "Totale");


      painter->setFont(QFont("Arial",10,50));
      painter->drawText(0,1900,2700,100,
                       Qt::AlignVCenter|Qt::AlignRight,
                       "Pagina: "+QString("%1").arg(pagNo)+"   ");
      painter->drawText(20,1900,2700,100,
                       Qt::AlignVCenter|Qt::AlignLeft,
                       "Lylibrary - versione "+settingsManager->generalValue("Version/version",QVariant()).toString());
      painter->drawLine(0,1900,2870,1900);
}

void Stampe::esporta_pdf(QPrinter *printer, QSqlRelationalTableModel *dataset)
{
    // la funzione stampaarticoli accoglie come parametro un oggetto puntatore
    // che rappresenta il dataset da mandare in stampa e che quindi
    // contiene i dati del report
    printer->setResolution(254);
     printer->setPageMargins(5.0,5.0,5.0,5.0,QPrinter::Millimeter);
      printer->setOrientation(QPrinter::Landscape);
    // inizio fase di stampa. Si attiva il painter sulla printer
    // impostata in precedenza
  painter->begin(printer);



  // si richiama la sotto funzione che "disegna" il modulo di stampa
  int pagNo = 0;
  ++pagNo;
  this->pagina(pagNo);


  int recNo = 0;

  // per ogni record del data set si effettua la stampa del dettaglio

  while (recNo < dataset->rowCount())
  {

        // Occorre gestire a mano il controllo di quante righe è possibile stampare
        // per prova si è impostato un massimo di 4 righe di dettaglio per pagina
        // Adeguare il numero di righe in base all'altezza del carattere di stampa

      for (int secRow = 0; secRow < 16; ++secRow)
      {
          QSqlRecord record = dataset->record(recNo);

          if (!record.isEmpty())
          {
             // in realt�  il tentativo di controllare se in record è vuoto
             // non funziona nel presente esempio. Non ho capito perché.

              int verticalPos = 200+100*(secRow+1);

              painter->setPen(QPen(Qt::black,1));
              painter->drawText(0,verticalPos,200,100,
                                Qt::AlignVCenter | Qt::AlignHCenter,
                           record.value("cod_articolo").toString());
              painter->drawText(200,verticalPos,400,100,
                                Qt::AlignVCenter|Qt::AlignHCenter,
                           record.value("codbarre").toString());
              painter->drawText(600,verticalPos,600,100,
                           Qt::AlignVCenter|Qt::AlignHCenter,
                           record.value("nome_articolo").toString());
              painter->drawText(1000,verticalPos,800,100,
                                Qt::AlignVCenter|Qt::AlignHCenter,
                                record.value("lingua").toString());
              painter->drawText(1400,verticalPos,1000,100,
                                Qt::AlignVCenter|Qt::AlignHCenter,
                                record.value("categ").toString());
              painter->drawText(1600,verticalPos,1400,100,
                                Qt::AlignVCenter|Qt::AlignHCenter,
                                record.value("scaffale").toString());
              painter->drawText(1800,verticalPos,1600,100,
                                Qt::AlignVCenter|Qt::AlignHCenter,
                                record.value("quantita").toString());
              ++recNo;
          }

      }

      // onde evitare di generare una pagina vuota si controlla a mano
      // se si è reggiunto l'ultimo record

      if(recNo < dataset->rowCount())
      {
          printer->newPage();
          this->pagina(pagNo);
          ++pagNo;
      }
   }

   // chiusura della fase di inserimento dati. Richiamando end()
   // il file viene fisicamente prodotto o mandato in stampa

  painter->end();
}

void Stampe::pagina(int pagNo)
{
    // La funzione disegna gli oggetti "Statici" presenti su ogni pagina
    // di fatto prepara il modulo di stampa per accogliere poi i dati

  painter->setFont(QFont("Arial",12,75));
  painter->setPen(QPen(Qt::blue,1));
  painter->drawRect(0,0,2870,2000);

  painter->setPen(QPen(Qt::blue,1));
  painter->drawRect(0,0,2870,120);
  painter->drawText(0,0,2870,120,
                   Qt::AlignVCenter|Qt::AlignHCenter,
                   "Carico magazzino");

  //Valorizzazione header box intestazione colonne

  painter->drawText(0,200,200,100,
                   Qt::AlignVCenter|Qt::AlignHCenter,
                   "ID");
  painter->drawText(200,200,400,100,
                   Qt::AlignVCenter|Qt::AlignHCenter,
                   "Barcode");
  painter->drawText(600,200,600,100,
                   Qt::AlignVCenter|Qt::AlignHCenter,
                   "Titolo");
  painter->drawText(1000,200,800,100,
                    Qt::AlignVCenter|Qt::AlignHCenter,
                    "Lingua");
  painter->drawText(1400,200,1000,100,
                    Qt::AlignVCenter|Qt::AlignHCenter,
                    "Categoria");
  painter->drawText(1600,200,1400,100,
                    Qt::AlignVCenter|Qt::AlignHCenter,
                    "Collocazione");
  painter->drawText(1800,200,1600,100,
                    Qt::AlignVCenter|Qt::AlignHCenter,
                    "Quantit�");

      painter->setFont(QFont("Arial",10,50));
      painter->drawText(0,1900,2700,100,
                       Qt::AlignVCenter|Qt::AlignRight,
                       "Pagina: "+QString("%1").arg(pagNo)+"   ");
      painter->drawText(0,1900,2700,100,
                       Qt::AlignVCenter|Qt::AlignLeft,
                       "Lylibrary - versione "+settingsManager->generalValue("Version/version",QVariant()).toString());
      painter->drawLine(0,1900,2870,1900);
}

void Stampe::esporta_scarico(QPrinter *printer, QSqlRelationalTableModel *dataset)
{
    // la funzione stampaarticoli accoglie come parametro un oggetto puntatore
    // che rappresenta il dataset da mandare in stampa e che quindi
    // contiene i dati del report
    printer->setResolution(254);
     printer->setPageMargins(5.0,5.0,5.0,5.0,QPrinter::Millimeter);
      printer->setOrientation(QPrinter::Landscape);
    // inizio fase di stampa. Si attiva il painter sulla printer
    // impostata in precedenza
  painter->begin(printer);



  // si richiama la sotto funzione che "disegna" il modulo di stampa
  int pagNo = 0;
  ++pagNo;
  this->pagina_scarico(pagNo);


  int recNo = 0;

  // per ogni record del data set si effettua la stampa del dettaglio

  while (recNo < dataset->rowCount())
  {

        // Occorre gestire a mano il controllo di quante righe è possibile stampare
        // per prova si è impostato un massimo di 4 righe di dettaglio per pagina
        // Adeguare il numero di righe in base all'altezza del carattere di stampa

      for (int secRow = 0; secRow < 16; ++secRow)
      {
          QSqlRecord record = dataset->record(recNo);

          if (!record.isEmpty())
          {
             // in realt�  il tentativo di controllare se in record è vuoto
             // non funziona nel presente esempio. Non ho capito perché.

              int verticalPos = 200+100*(secRow+1);

              painter->setPen(QPen(Qt::black,1));
              painter->drawText(0,verticalPos,200,100,
                                Qt::AlignVCenter | Qt::AlignHCenter,
                           record.value("cod_articolo").toString());
              painter->drawText(200,verticalPos,400,100,
                                Qt::AlignVCenter|Qt::AlignHCenter,
                           record.value("codbarre").toString());
              painter->drawText(600,verticalPos,600,100,
                           Qt::AlignVCenter|Qt::AlignHCenter,
                           record.value("nome_articolo").toString());
              painter->drawText(1000,verticalPos,800,100,
                                Qt::AlignVCenter|Qt::AlignHCenter,
                                record.value("lingua").toString());
              painter->drawText(1400,verticalPos,1000,100,
                                Qt::AlignVCenter|Qt::AlignHCenter,
                                record.value("categ").toString());
              painter->drawText(1600,verticalPos,1400,100,
                                Qt::AlignVCenter|Qt::AlignHCenter,
                                record.value("scaffale").toString());
              painter->drawText(1800,verticalPos,1600,100,
                                Qt::AlignVCenter|Qt::AlignHCenter,
                                record.value("quantita").toString());
              ++recNo;
          }

      }

      // onde evitare di generare una pagina vuota si controlla a mano
      // se si è reggiunto l'ultimo record

      if(recNo < dataset->rowCount())
      {
          printer->newPage();
          this->pagina_scarico(pagNo);
          ++pagNo;
      }
   }

   // chiusura della fase di inserimento dati. Richiamando end()
   // il file viene fisicamente prodotto o mandato in stampa

  painter->end();
}

void Stampe::pagina_scarico(int pagNo)
{
    // La funzione disegna gli oggetti "Statici" presenti su ogni pagina
    // di fatto prepara il modulo di stampa per accogliere poi i dati

  painter->setFont(QFont("Arial",12,75));
  painter->setPen(QPen(Qt::blue,1));
  painter->drawRect(0,0,2870,2000);

  painter->setPen(QPen(Qt::blue,1));
  painter->drawRect(0,0,2870,120);
  painter->drawText(0,0,2870,120,
                   Qt::AlignVCenter|Qt::AlignHCenter,
                   "Scarico magazzino");

  //Valorizzazione header box intestazione colonne

  painter->drawText(0,200,200,100,
                   Qt::AlignVCenter|Qt::AlignHCenter,
                   "ID");
  painter->drawText(200,200,400,100,
                   Qt::AlignVCenter|Qt::AlignHCenter,
                   "Barcode");
  painter->drawText(600,200,600,100,
                   Qt::AlignVCenter|Qt::AlignHCenter,
                   "Titolo");
  painter->drawText(1000,200,800,100,
                    Qt::AlignVCenter|Qt::AlignHCenter,
                    "Lingua");
  painter->drawText(1400,200,1000,100,
                    Qt::AlignVCenter|Qt::AlignHCenter,
                    "Categoria");
  painter->drawText(1600,200,1400,100,
                    Qt::AlignVCenter|Qt::AlignHCenter,
                    "Collocazione");
  painter->drawText(1800,200,1600,100,
                    Qt::AlignVCenter|Qt::AlignHCenter,
                    "Quantit�");

      painter->setFont(QFont("Arial",10,50));
      painter->drawText(0,1900,2700,100,
                       Qt::AlignVCenter|Qt::AlignRight,
                       "Pagina: "+QString("%1").arg(pagNo)+"   ");
      painter->drawText(0,1900,2700,100,
                       Qt::AlignVCenter|Qt::AlignLeft,
                       "Lylibrary - versione "+settingsManager->generalValue("Version/version",QVariant()).toString());
      painter->drawLine(0,1900,2870,1900);
}

void Stampe::stampa_fatt_vendita_libri(QPrinter *printer, QSqlRelationalTableModel *dataset, QModelIndex index, int id)
{
    // la funzione stampa_fatt_vendita_libri accoglie come parametro un oggetto puntatore
        // che rappresenta il dataset da mandare in stampa e che quindi
        // contiene i dati del report
          printer->setPageMargins(5.0,5.0,5.0,5.0,QPrinter::Millimeter);
          printer->setPaperSize(QPrinter::A4);
          printer->setResolution(254);
          printer->setOrientation(QPrinter::Portrait);
        // inizio fase di stampa. Si attiva il painter sulla printer
        // impostata in precedenza
      painter->begin(printer);

      painter->setFont(QFont("Arial",8,50));
      painter->setPen(QPen(Qt::black,1));

      int pagNo = 1;
      pagina_fatt_vendita_libri(pagNo,id);
      ++pagNo;

      int recNo = 0;
      if(index.row() < dataset->rowCount())
      {
          //Instestazione Fattura immediata libri
          intestazione();
          for(int secRow=0; secRow < 1; ++secRow){
              QSqlRecord rec = dataset->record(index.row());
              QSqlQuery query1;
              query1.prepare("select * from fatture_vendita_righe where id='"+QString::number(id)+"'");
              if(!rec.isEmpty()){
                  if(query1.exec()){
                  if(query1.next()){

                      painter->setFont(QFont("Arial",16,50));
                      painter->setPen(QPen(Qt::RoundCap));
                      painter->drawText(1190,65,2000,1500,
                                        Qt::AlignJustify | Qt::AlignJustify,
                                        query1.value(13).toString());
                      painter->setFont(QFont("Arial",8,50));
                      painter->drawText(1190,125,4000,100,
                                      Qt::AlignJustify | Qt::AlignJustify,
                                      query1.value(14).toString()+" "+query1.value(15).toString()+"-"+query1.value(16).toString());
                      painter->drawText(1190,160,4000,100,
                                    Qt::AlignJustify | Qt::AlignJustify,
                                    "Tel. "+query1.value(18).toString()+" Fax "+query1.value(19).toString());
                      painter->drawText(1190,205,4000,100,
                                       Qt::AlignJustify | Qt::AlignJustify,
                                      "C.F/P.I: "+query1.value(20).toString());
                  }

                  painter->setFont(QFont("Arial",8,50));
                  painter->drawText(1190,345,4000,100,
                                    Qt::AlignJustify | Qt::AlignJustify,
                                    FATTURA_IMMEDIATA"  N�:  "+rec.value("id").toString());
                  painter->drawText(1190,405,4000,100,
                                    Qt::AlignJustify | Qt::AlignJustify,
                                    DATA":   "+rec.value("data").toDate().toString("dd-MM-yyyy"));
                  painter->drawText(1190,460,4000,100,
                                    Qt::AlignJustify | Qt::AlignJustify,
                                    TIPO_FATTURA":   "+rec.value("tipo_fattura").toString());
              }
              }

          }
      }
      QSqlQuery query;
      query.prepare("select * from fatture_righe_vendita_art where id='"+QString::number(id)+"'");
      if(query.exec()){
      if(recNo < query.record().count()){
          for(int row_sec=0; row_sec < 36; ++row_sec){

              if(query.next()){
              QSqlRecord rec = query.record();
              int verticalPos = 595+45*(row_sec+1);

              if(!rec.isEmpty()){

                  painter->setFont(QFont("Arial",8,50));
                  painter->drawText(20,verticalPos,200,100,
                                    Qt::AlignJustify | Qt::AlignJustify,
                                    rec.value("cod_art").toString());
                  painter->drawText(225,verticalPos,800,100,
                                    Qt::AlignJustify | Qt::AlignJustify,
                                    rec.value("descrizione").toString());
                  painter->drawText(1020,verticalPos,200,100,
                                    Qt::AlignJustify | Qt::AlignJustify,
                                    rec.value("unita").toString());
                  painter->drawText(1102,verticalPos,200,100,
                                    Qt::AlignJustify | Qt::AlignJustify,
                                    rec.value("quantita").toString());
                  painter->drawText(1270,verticalPos,200,100,
                                    Qt::AlignJustify | Qt::AlignJustify,
                                    QString::number(rec.value("prezzo_unit").toDouble())+".00");
                  painter->drawText(1470,verticalPos,200,100,
                                    Qt::AlignJustify | Qt::AlignJustify,
                                    QString::number(rec.value("sconto").toDouble())+".00");
                  painter->drawText(1700,verticalPos,200,100,
                                    Qt::AlignJustify | Qt::AlignJustify,
                                    QString::number(rec.value("prezzo_s_iva").toDouble())+".00");
                  painter->drawText(1930,verticalPos,200,100,
                                    Qt::AlignJustify | Qt::AlignJustify,
                                    QString::number(rec.value("iva").toDouble())+"%");

              }
              }
              ++recNo;
          }
      }
      }

      if(recNo < query.record().count()){
          painter->drawText(1600,2590,4000,100,
                                     Qt::AlignJustify | Qt::AlignJustify,
                                     "Segue ----->");
                   printer->newPage();
                   pagina_fatt_vendita_libri(pagNo,id);
                   ++pagNo;
      }
        painter->end();
}

void Stampe::pagina_fatt_vendita_libri(int pagNo,int id)
{
    //Instestazione Fattura immediata libri
        intestazione();
        //Impaginazione fattura
        layout_fattura();
        int recNo_1=0;
        //Impostazione degli ultimi dati
        QSqlQuery query2;
        query2.exec("select * from fatture_vendita_righe where id='"+QString::number(id)+"'");
        if(recNo_1 < query2.record().count()){
            for(int x=0; x < 1; ++x){
                if(query2.next()){
                QSqlRecord rec = query2.record();
                //int verticalPos = 2500+1*(x+1);

                if(!rec.isEmpty()){

                    //Visualizza le scadenze
                    painter->drawText(820,2555,4000,100,
                                      Qt::AlignJustify | Qt::AlignJustify,
                                      rec.value("scadenza_1").toDate().toString("dd-MM-yyyy"));

                    painter->drawText(820,2580,4000,100,
                                      Qt::AlignJustify | Qt::AlignJustify,
                                      rec.value("scadenza_2").toDate().toString("dd-MM-yyyy"));

                    painter->drawText(820,2605,4000,100,
                                      Qt::AlignJustify | Qt::AlignJustify,
                                      rec.value("scadenza_3").toDate().toString("dd-MM-yyyy"));

                    //Visualizza importi
                    painter->drawText(1075,2555,4000,100,
                                      Qt::AlignJustify | Qt::AlignJustify,
                                      QString::fromUtf8("\u20AC")+ "   " +rec.value("importo_1").toString());
                    painter->drawText(1075,2580,4000,100,
                                      Qt::AlignJustify | Qt::AlignJustify,
                                      QString::fromUtf8("\u20AC")+ "   " +rec.value("importo_2").toString());
                    painter->drawText(1075,2605,4000,100,
                                      Qt::AlignJustify | Qt::AlignJustify,
                                      QString::fromUtf8("\u20AC")+ "   " +rec.value("importo_3").toString());

                    //Visualizza spese di trasporto
                    painter->drawText(355,2260,4000,100,
                                      Qt::AlignJustify | Qt::AlignJustify,
                                      QString::fromUtf8("\u20AC")+ "   " +rec.value("spese_trasporto").toString());

                    //Visualizza spese di incasso
                    painter->drawText(975,2260,4000,100,
                                      Qt::AlignJustify | Qt::AlignJustify,
                                      QString::fromUtf8("\u20AC")+ "   " +rec.value("spese_incasso").toString());

                    //Visualizza tipo di pagamento
                    painter->drawText(160,2503,4000,100,
                                      Qt::AlignJustify | Qt::AlignJustify,
                                      rec.value("tipo_pagamento").toString());

                    //Visualizza Banca
                    painter->drawText(110,2553,4000,100,
                                      Qt::AlignJustify | Qt::AlignJustify,
                                      rec.value("banca").toString());

                    //Visualizza Iban
                    painter->drawText(90,2608,4000,100,
                                      Qt::AlignJustify | Qt::AlignJustify,
                                      rec.value("iban").toString());

                    //Visualizza note fattura
                    painter->drawText(160,2503,4000,100,
                                      Qt::AlignJustify | Qt::AlignJustify,
                                      rec.value("annotazioni").toString());


                    //Setto i caratteri in grassetto
                    painter->setFont(QFont("Arial",6,QFont::Bold,true ));
                    //Visualizza imponibile
                    painter->drawText(1670,2290,4000,100,
                                      Qt::AlignJustify | Qt::AlignJustify,
                                      QString::fromUtf8("\u20AC")+ "   " +rec.value("prezzo_s_iva").toString());

                    //Visualizza iva
                    painter->drawText(1670,2370,4000,100,
                                      Qt::AlignJustify | Qt::AlignJustify,
                                      QString::fromUtf8("\u20AC")+ "   " +rec.value("iva").toString());

                    //Visualizzo totale
                    painter->drawText(1670,2450,4000,100,
                                      Qt::AlignJustify | Qt::AlignJustify,
                                      QString::fromUtf8("\u20AC")+ "   " +rec.value("totale").toString());

                    //Setto i caratteri in corsivo

                    QFont *font = new QFont("Arial");
                    font->setItalic(true);
                    font->setPixelSize(24);
                    painter->setFont(*font);

                    //Visualizzo descizione iva
                    QSqlQuery query3;
                    query3.exec("select A.iva,A.descrizione as aliquota, B.iva as fatture_righe_vendita_art from aliquota A,fatture_righe_vendita_art B where A.iva=B.iva and B.id='"+QString::number(id)+"'");
                    if(query3.next()){

                        //Visualizzo codice iva
                        painter->drawText(20,2360,4000,100,
                                          Qt::AlignJustify | Qt::AlignJustify,
                                          query3.value(0).toString());

                        //Visualizzo descrizione iva
                        painter->drawText(280,2360,4000,100,
                                          Qt::AlignJustify | Qt::AlignJustify,
                                          query3.value(1).toString());


                        //Visualizza imponibile
                        painter->drawText(540,2360,4000,100,
                                          Qt::AlignJustify | Qt::AlignJustify,
                                          QString::fromUtf8("\u20AC")+ "   " +rec.value("prezzo_s_iva").toString());


                        //Visualizzo codice iva
                        painter->drawText(800,2360,4000,100,
                                          Qt::AlignJustify | Qt::AlignJustify,
                                          query3.value(0).toString()+"%");



                        //Visualizza iva
                        painter->drawText(1060,2360,4000,100,
                                          Qt::AlignJustify | Qt::AlignJustify,
                                          QString::fromUtf8("\u20AC")+ "   " +rec.value("iva").toString());

                    }


                }
                }
                ++recNo_1;
            }
        }
        //Numero di pagina
        painter->setFont(QFont("Arial",6,50));
        painter->drawText(1800,2700,2000,100,
                         Qt::AlignVCenter|Qt::AlignLeft,
                          "Pagina: "+QString("%1").arg(pagNo)+"/"+QString("%1").arg(pagNo));

}

void Stampe::stampa_fatt_vendita_prod_dig(QPrinter *printer, QSqlRelationalTableModel *dataset, QModelIndex index, int id)
{
    // la funzione stampa_fatt_vendita_libri accoglie come parametro un oggetto puntatore
          // che rappresenta il dataset da mandare in stampa e che quindi
          // contiene i dati del report
            printer->setPageMargins(5.0,5.0,5.0,5.0,QPrinter::Millimeter);
            printer->setPaperSize(QPrinter::A4);
            printer->setResolution(254);
            printer->setOrientation(QPrinter::Portrait);
          // inizio fase di stampa. Si attiva il painter sulla printer
          // impostata in precedenza
        painter->begin(printer);

        painter->setFont(QFont("Arial",8,50));
        painter->setPen(QPen(Qt::black,1));

        int pagNo = 1;
        pagina_fatt_vendita_prod_dig(pagNo,id);
        ++pagNo;

        int recNo = 0;
        if(index.row() < dataset->rowCount())
        {

            for(int secRow=0; secRow < 1; ++secRow){
                QSqlRecord rec = dataset->record(index.row());
                QSqlQuery query1;
                query1.exec("select * from fatture_vendita_righe_dig where id='"+QString::number(id)+"'");
                if(!rec.isEmpty()){

                    if(query1.next()){

                        painter->setFont(QFont("Arial",16,50));
                        painter->setPen(QPen(Qt::RoundCap));
                        painter->drawText(1190,65,2000,1500,
                                          Qt::AlignJustify | Qt::AlignJustify,
                                          query1.value(13).toString());
                        painter->setFont(QFont("Arial",8,50));
                        painter->drawText(1190,125,4000,100,
                                        Qt::AlignJustify | Qt::AlignJustify,
                                        query1.value(14).toString()+" "+query1.value(15).toString()+"-"+query1.value(16).toString());
                        painter->drawText(1190,160,4000,100,
                                      Qt::AlignJustify | Qt::AlignJustify,
                                      "Tel. "+query1.value(18).toString()+" Fax "+query1.value(19).toString());
                        painter->drawText(1190,205,4000,100,
                                         Qt::AlignJustify | Qt::AlignJustify,
                                        "C.F/P.I: "+query1.value(20).toString());
                    }

                    painter->setFont(QFont("Arial",8,50));
                    painter->drawText(1190,345,4000,100,
                                      Qt::AlignJustify | Qt::AlignJustify,
                                      FATTURA_IMMEDIATA"  N�:  "+rec.value("id").toString());
                    painter->drawText(1190,405,4000,100,
                                      Qt::AlignJustify | Qt::AlignJustify,
                                      DATA":   "+rec.value("data").toDate().toString("dd-MM-yyyy"));
                    painter->drawText(1190,460,4000,100,
                                      Qt::AlignJustify | Qt::AlignJustify,
                                      TIPO_FATTURA":   "+rec.value("tipo_fattura").toString());
                }

            }
        }
        QSqlQuery query;
        query.exec("select * from fatture_righe_vendita_prod_dig where id='"+QString::number(id)+"'");
        if(recNo < query.record().count()){
            for(int row_sec=0; row_sec < 36; ++row_sec){

                if(query.next()){
                QSqlRecord rec = query.record();
                int verticalPos = 595+45*(row_sec+1);

                if(!rec.isEmpty()){

                    painter->setFont(QFont("Arial",8,50));
                    painter->drawText(20,verticalPos,200,100,
                                      Qt::AlignJustify | Qt::AlignJustify,
                                      rec.value("cod_art").toString());

                    QString desc = rec.value("descrizione").toString();
                    if(desc.length() == 100)
                    {
                        desc.append("\n");
                    }
                    painter->drawText(225,verticalPos,800,100,
                                      Qt::AlignJustify | Qt::AlignJustify,
                                      desc);


                    painter->drawText(1020,verticalPos,200,100,
                                      Qt::AlignJustify | Qt::AlignJustify,
                                      rec.value("unita").toString());
                    painter->drawText(1102,verticalPos,200,100,
                                      Qt::AlignJustify | Qt::AlignJustify,
                                      rec.value("quantita").toString());
                    painter->drawText(1270,verticalPos,200,100,
                                      Qt::AlignJustify | Qt::AlignJustify,
                                      QString::number(rec.value("prezzo_unit").toDouble())+".00");
                    painter->drawText(1470,verticalPos,200,100,
                                      Qt::AlignJustify | Qt::AlignJustify,
                                      QString::number(rec.value("sconto").toDouble())+".00");
                    painter->drawText(1700,verticalPos,200,100,
                                      Qt::AlignJustify | Qt::AlignJustify,
                                      QString::number(rec.value("prezzo_s_iva").toDouble())+".00");
                    painter->drawText(1930,verticalPos,200,100,
                                      Qt::AlignJustify | Qt::AlignJustify,
                                      QString::number(rec.value("iva").toDouble())+"%");

                }
                }
                ++recNo;
            }
        }

        if(recNo < query.record().count()){
            painter->drawText(1600,2590,4000,100,
                              Qt::AlignJustify | Qt::AlignJustify,
                              "Segue ----->");
            printer->newPage();
            pagina_fatt_vendita_prod_dig(pagNo,id);
            ++pagNo;
        }


          painter->end();
}

void Stampe::pagina_fatt_vendita_prod_dig(int pagNo, int id)
{
    //Instestazione Fattura immediata libri
       intestazione();
       //Impaginazione fattura
       layout_fattura();
       int recNo_1=0;
       //Impostazione degli ultimi dati
       QSqlQuery query2;
       query2.exec("select * from fatture_vendita_righe_dig where id='"+QString::number(id)+"'");
       if(recNo_1 < query2.record().count()){
           for(int x=0; x < 1; ++x){
               if(query2.next()){
               QSqlRecord rec = query2.record();
               //int verticalPos = 2500+1*(x+1);

               if(!rec.isEmpty()){

                   //Visualizza le scadenze
                   painter->drawText(820,2555,4000,100,
                                     Qt::AlignJustify | Qt::AlignJustify,
                                     rec.value("scadenza_1").toDate().toString("dd-MM-yyyy"));

                   painter->drawText(820,2580,4000,100,
                                     Qt::AlignJustify | Qt::AlignJustify,
                                     rec.value("scadenza_2").toDate().toString("dd-MM-yyyy"));

                   painter->drawText(820,2605,4000,100,
                                     Qt::AlignJustify | Qt::AlignJustify,
                                     rec.value("scadenza_3").toDate().toString("dd-MM-yyyy"));

                   //Visualizza importi
                   painter->drawText(1075,2555,4000,100,
                                     Qt::AlignJustify | Qt::AlignJustify,
                                     QString::fromUtf8("\u20AC")+ "   " +rec.value("importo_1").toString());
                   painter->drawText(1075,2580,4000,100,
                                     Qt::AlignJustify | Qt::AlignJustify,
                                     QString::fromUtf8("\u20AC")+ "   " +rec.value("importo_2").toString());
                   painter->drawText(1075,2605,4000,100,
                                     Qt::AlignJustify | Qt::AlignJustify,
                                     QString::fromUtf8("\u20AC")+ "   " +rec.value("importo_3").toString());

                   //Visualizza spese di trasporto
                   painter->drawText(355,2260,4000,100,
                                     Qt::AlignJustify | Qt::AlignJustify,
                                     QString::fromUtf8("\u20AC")+ "   " +rec.value("spese_trasporto").toString());

                   //Visualizza spese di incasso
                   painter->drawText(975,2260,4000,100,
                                     Qt::AlignJustify | Qt::AlignJustify,
                                     QString::fromUtf8("\u20AC")+ "   " +rec.value("spese_incasso").toString());

                   //Visualizza tipo di pagamento
                   painter->drawText(160,2503,4000,100,
                                     Qt::AlignJustify | Qt::AlignJustify,
                                     rec.value("tipo_pagamento").toString());

                   //Visualizza Banca
                   painter->drawText(110,2553,4000,100,
                                     Qt::AlignJustify | Qt::AlignJustify,
                                     rec.value("banca").toString());

                   //Visualizza Iban
                   painter->drawText(90,2608,4000,100,
                                     Qt::AlignJustify | Qt::AlignJustify,
                                     rec.value("iban").toString());

                   //Visualizza note fattura
                   painter->drawText(160,2503,4000,100,
                                     Qt::AlignJustify | Qt::AlignJustify,
                                     rec.value("annotazioni").toString());


                   //Setto i caratteri in grassetto
                   painter->setFont(QFont("Arial",6,QFont::Bold,true ));
                   //Visualizza imponibile
                   painter->drawText(1670,2290,4000,100,
                                     Qt::AlignJustify | Qt::AlignJustify,
                                     QString::fromUtf8("\u20AC")+ "   " +rec.value("prezzo_s_iva").toString());

                   //Visualizza iva
                   painter->drawText(1670,2370,4000,100,
                                     Qt::AlignJustify | Qt::AlignJustify,
                                     QString::fromUtf8("\u20AC")+ "   " +rec.value("iva").toString());

                   //Visualizzo totale
                   painter->drawText(1670,2450,4000,100,
                                     Qt::AlignJustify | Qt::AlignJustify,
                                     QString::fromUtf8("\u20AC")+ "   " +rec.value("totale").toString());

                   //Setto i caratteri in corsivo

                   QFont *font = new QFont("Arial");
                   font->setItalic(true);
                   font->setPixelSize(24);
                   painter->setFont(*font);

                   //Visualizzo descizione iva
                   QSqlQuery query3;
                   query3.exec("select A.iva,A.descrizione as aliquota, B.iva as fatture_righe_vendita_prod_dig from aliquota A,fatture_righe_vendita_prod_dig B where A.iva=B.iva and B.id='"+QString::number(id)+"'");
                   if(query3.next()){

                       //Visualizzo codice iva
                       painter->drawText(20,2360,4000,100,
                                         Qt::AlignJustify | Qt::AlignJustify,
                                         query3.value(0).toString());

                       //Visualizzo descrizione iva
                       painter->drawText(280,2360,4000,100,
                                         Qt::AlignJustify | Qt::AlignJustify,
                                         query3.value(1).toString());


                       //Visualizza imponibile
                       painter->drawText(540,2360,4000,100,
                                         Qt::AlignJustify | Qt::AlignJustify,
                                         QString::fromUtf8("\u20AC")+ "   " +rec.value("prezzo_s_iva").toString());


                       //Visualizzo codice iva
                       painter->drawText(800,2360,4000,100,
                                         Qt::AlignJustify | Qt::AlignJustify,
                                         query3.value(0).toString()+"%");



                       //Visualizza iva
                       painter->drawText(1060,2360,4000,100,
                                         Qt::AlignJustify | Qt::AlignJustify,
                                         QString::fromUtf8("\u20AC")+ "   " +rec.value("iva").toString());

                   }


               }
               }
               ++recNo_1;
           }
       }
       //Numero di pagina
       painter->setFont(QFont("Arial",6,50));
       painter->drawText(1800,2700,2000,100,
                        Qt::AlignVCenter|Qt::AlignLeft,
                         "Pagina: "+QString("%1").arg(pagNo)+"/"+QString("%1").arg(pagNo));
}

void Stampe::intestazione()
{
    //Instestazione
    QRect target(105,10,500,240);
    QImage img(settingsManager->generalValue("Image_resize/dir_image",QVariant()).toString());
    painter->drawImage(target,img);

    QSqlQuery query;
    query.exec("select * from azienda");
    if(query.next())
    {
        painter->setFont(QFont("Arial",16,50));
        painter->setPen(QPen(Qt::RoundCap));
        painter->drawText(55,250,2000,100,
                          Qt::AlignJustify | Qt::AlignJustify,
                          query.value(0).toString());
        painter->setFont(QFont("Arial",8,50));
        painter->drawText(55,315,4000,100,
                        Qt::AlignJustify | Qt::AlignJustify,
                        query.value(3).toString()+" "+query.value(4).toString()+"-"+query.value(5).toString()+
                        " "+"("+query.value(6).toString()+")");
        painter->drawText(55,360,4000,100,
                      Qt::AlignJustify | Qt::AlignJustify,
                      "Tel. "+query.value(7).toString()+" Fax "+query.value(8).toString());
        painter->drawText(55,405,4000,100,
                         Qt::AlignJustify | Qt::AlignJustify,
                        "e-mail: "+query.value(10).toString()+" Internet: "+query.value(9).toString());
        painter->drawText(55,445,4000,100,
                         Qt::AlignJustify | Qt::AlignJustify,
                        "C.F: "+query.value(2).toString()+" P.Iva: "+query.value(1).toString());
    }
}

void Stampe::layout_fattura()
{

    //Rettangolo intestazione azienda
    QRect rett(32,220,715,305);
    painter->drawRoundRect(rett,25,25);
    painter->drawLine(55,310,680,310);

    //Rettangolo intestazione cliente-fornitore
    QRect rett1(1170,45,725,205);
    painter->drawRoundRect(rett1,25,25);
    painter->drawLine(1190,123,1815,123);
    painter->setFont(QFont("Arial",10,50));
    painter->drawText(1235,10,2000,1500,
                      Qt::AlignJustify | Qt::AlignJustify,
                      "Cliente");

    //Rettangolo dati fattura
    QRect rett2(1170,325,725,205);
    painter->drawRoundRect(rett2,25,25);
    painter->setFont(QFont("Arial",10,50));
    painter->drawText(1235,280,2000,1500,
                      Qt::AlignJustify | Qt::AlignJustify,
                      "Dati Fattura");

    //Rettangolo fattura da riempire
    painter->drawRect(5,550,2000,2140);
    painter->drawRect(5,610,2000,0);

    painter->setFont(QFont("Arial",6,50));
    //Rettangolo Codice
    painter->drawRect(5,550,200,1700);
    painter->drawText(12,540,2000,100,
                      Qt::AlignVCenter|Qt::AlignLeft,
                      CODICE);
    //Rettangolo Descrizione
    painter->drawRect(205,550,800,1700);
    painter->drawText(225,540,2000,100,
                      Qt::AlignVCenter|Qt::AlignLeft,
                      DESCRIZIONE);

    //Rettangolo UM
    painter->drawRect(1005,550,80,1700);
    painter->drawText(1020,540,2000,100,
                      Qt::AlignVCenter|Qt::AlignLeft,
                      UM);

    //Rettangolo Quantit�
    painter->drawRect(1085,550,160,1700);
    painter->drawText(1102,540,2000,100,
                      Qt::AlignVCenter|Qt::AlignLeft,
                      QString::fromUtf8(QUANTITA));

    //Rettangolo Prezzo
    painter->drawRect(1245,550,200,1700);
    painter->drawText(1290,540,2000,100,
                      Qt::AlignVCenter|Qt::AlignLeft,
                      PREZZO_UNITARIO);

    //Rettangolo Sconto
    painter->drawRect(1445,550,200,1700);
    painter->drawText(1490,540,2000,100,
                      Qt::AlignVCenter|Qt::AlignLeft,
                      SCONTO);

    //Rettangolo importo
    painter->drawRect(1645,550,255,1700);
    painter->drawText(1720,540,2000,100,
                      Qt::AlignVCenter|Qt::AlignLeft,
                      IMPORTO);

    //Rettangolo Iva
    painter->drawRect(1900,550,105,1700);
    painter->drawText(1930,540,2000,100,
                      Qt::AlignVCenter|Qt::AlignLeft,
                      IVA);

    //Rettangolo spese di trasporto
    painter->drawRect(5,2250,1300,50);
    painter->drawRect(325,2250,320,50);
    painter->drawRect(645,2250,320,50);
    painter->drawText(20,2225,2000,100,
                      Qt::AlignVCenter|Qt::AlignLeft,
                      SPESE_DI_TRASPORTO);

    painter->drawText(655,2225,2000,100,
                      Qt::AlignVCenter|Qt::AlignLeft,
                      SPESE_DI_INCASSO);

    //Dettaglio iva
    painter->drawRect(5,2300,1300,50);
    painter->drawRect(5,2300,1300,190);
    painter->drawRect(5,2300,260,190);
    painter->drawRect(265,2300,260,190);
    painter->drawRect(525,2300,260,190);
    painter->drawRect(785,2300,260,190);

    painter->drawText(20,2275,2000,100,
                      Qt::AlignVCenter|Qt::AlignLeft,
                      DETTAGLIO_IVA);

    painter->drawText(280,2275,2000,100,
                      Qt::AlignVCenter|Qt::AlignLeft,
                      DESCRIZIONE);

    painter->drawText(540,2275,2000,100,
                      Qt::AlignVCenter|Qt::AlignLeft,
                      IMPONIBILE);

    painter->drawText(800,2275,2000,100,
                      Qt::AlignVCenter|Qt::AlignLeft,
                      IVA_PERC);

    painter->drawText(1060,2275,2000,100,
                      Qt::AlignVCenter|Qt::AlignLeft,
                      IMPOSTA);


    //Tipo pagamento
    painter->drawRect(5,2490,800,50);
    painter->drawText(20,2465,2000,100,
                      Qt::AlignVCenter|Qt::AlignLeft,
                      PAGAMENTO": ");

    //Banca
    painter->drawRect(5,2540,800,50);
    painter->drawText(20,2515,2000,100,
                      Qt::AlignVCenter|Qt::AlignLeft,
                      BANCA": ");

    //Iban
    painter->drawRect(5,2590,800,50);
    painter->drawText(20,2570,2000,100,
                      Qt::AlignVCenter|Qt::AlignLeft,
                      IBAN": ");

    //Note fattura
    painter->drawText(20,2620,2000,100,
                      Qt::AlignVCenter|Qt::AlignLeft,
                      NOTE_FATTURA": ");
    //Scadenze
    painter->drawRect(805,2490,500,200);
    painter->drawRect(805,2515,250,175);
    painter->drawRect(805,2490,500,25);
    painter->drawRect(805,2515,500,25);

    painter->drawText(1015,2454,2000,100,
                      Qt::AlignVCenter|Qt::AlignLeft,
                      SCADENZE);

    painter->drawText(820,2477,2000,100,
                      Qt::AlignVCenter|Qt::AlignLeft,
                      DATA_SC);

    painter->drawText(1075,2477,2000,100,
                      Qt::AlignVCenter|Qt::AlignLeft,
                      IMPORTO);

    //Rettangolo imponibile-iva-totale
    painter->drawRect(1305,2250,700,240);
    painter->drawRect(1305,2250,350,240);
    painter->drawText(1320,2250,2000,100,
                      Qt::AlignVCenter|Qt::AlignLeft,
                      IMPONIBILE);
    //Riga imponibile
    painter->drawRect(1305,2250,700,80);
    painter->drawText(1320,2330,2000,100,
                      Qt::AlignVCenter|Qt::AlignLeft,
                      IVA);
    //Riga iva
    painter->drawRect(1305,2330.6,700,80);
    painter->drawText(1320,2410,2000,100,
                      Qt::AlignVCenter|Qt::AlignLeft,
                      TOTALE);

    //Versione LyLibrary sulla pagina di stampa
    painter->setFont(QFont("Arial",6,50));
    painter->drawText(10,2700,2000,100,
                     Qt::AlignVCenter|Qt::AlignLeft,
                      "Lylibrary - versione "+settingsManager->generalValue("Version/version",QVariant()).toString()+" Powered by Codelinsoft");

}

void Stampe::print_codice(QPrinter *printer, QImage img, QString cognome, QString nome,
                         bool maschio, bool femmina, QString com_st, QString pr_st, QString codice, QDate date)
{
        printer->setPaperSize(QPrinter::A7);
        printer->setOrientation(QPrinter::Landscape);
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
