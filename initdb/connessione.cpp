#include "connessione.h"
#include "ui_connessione.h"
#include <QtSql>
#include <QSqlDatabase>
#include <QMessageBox>
#include <QTextStream>
#include <stdio.h>
#include <stdlib.h>
#include <QSettings>
#include <QtGui/QtGui>
#include <QProcess>


connessione::connessione(QWidget *parent) :
    QDialog(parent)
{
    setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose);
    setWindowFlags(Qt::Dialog | Qt::WindowMaximizeButtonHint | Qt::WindowMinimizeButtonHint);
    connect(p_crea,SIGNAL(clicked()),this,SLOT(connetti()));
    connect(open_db,SIGNAL(clicked()),this,SLOT(connettidatabase()));
    connect(add_mysql_socket,SIGNAL(clicked(bool)),this,SLOT(add_vis_groupbox(bool)));
    timer = new QTimer(this);
    prog=0;
    textEdit->setReadOnly(true);
    setModal(true);
#ifdef Q_WS_WIN
    settingsDir = new QDir(QDir::homePath()+"/lylibrary/");
#else
    settingsDir = new QDir(QDir::homePath()+"/.lylibrary/");
#endif

    general = new QSettings(settingsDir->path()+"/config.conf",QSettings::IniFormat);
    password->setEchoMode(QLineEdit::Password);
    pwd_lan->setEchoMode(QLineEdit::Password);
    confread();
    load_style(general->value("Tema/sel_tema",QVariant()).toByteArray());
}

connessione::~connessione()
{
}

QSqlDatabase connessione::connetti(){

    open_db->setEnabled(false);
    ip_dat->setEnabled(false);
    port->setEnabled(false);
    n_dat_lan->setEnabled(false);
    user->setEnabled(false);
    pwd_lan->setEnabled(false);
    QString db_lylibrary = "lylibrary";
    db = QSqlDatabase::addDatabase("QMYSQL");
        db.setHostName("localhost");
        db.setDatabaseName("mysql");
        db.setUserName("root");
        db.setPassword(password->text());

        if(!db.open()){

        }
        else{
            db.open();
            creadb(db,db_lylibrary);
            QDate date = QDate::currentDate();
            QSqlQuery query("select anno_ins from anno");
            query.exec();
            if(query.next()){
            }
            else{
                if(query.value(0).toString() == ""){
                    db.exec("INSERT INTO anno(anno_ins) VALUES("+QString::number(date.year())+")");
                    setdate(date.year());
                }
                else{
                    qDebug() << "|-----------INSERIMENTO NON EFFETTUATO--------------|";
                }
            }
        }

    return db;
}

void connessione::setdate(int year){
    general->setValue("Esercizio/anno",year);
}

bool connessione::isNull(){
    return FALSE;
}
void connessione::creadb(QSqlDatabase db, QString db_lylibrary){


        if (db.open())
        {
            QSqlQuery query;
            connect(timer, SIGNAL(timeout()), this, SLOT(barra_progresso()));
            timer->start();
            query.exec("create database "+db_lylibrary+" CHARACTER SET=utf8");
            textEdit->append("Creazione database lylibrary effettuata....");
            db.close();
            barra_progresso();

            db.setDatabaseName(db_lylibrary);
            db.open();
            textEdit->append("Creazione database Lylibrary effettuata...");

            query.exec("CREATE TABLE "+db_lylibrary+".idzero "
                       "(id INT NOT NULL, "
                       "text char(40) NOT NULL, "
                       "PRIMARY KEY(id))"
                       "ENGINE = MyISAM"
                       );
            query.exec("INSERT INTO idzero VALUES('0','')");
            textEdit->append(tr("Creazione tabella effettuata..."));
            query.exec("CREATE TABLE "+db_lylibrary+".clienti "
                       " (id INT NOT NULL , "
                       " nome char (30) NOT NULL, "
                       " cognome char (30) NOT NULL, "
                       " indirizzo char (30) NOT NULL, "
                       " telefono char (30) NOT NULL, "
                       " email char (30) NOT NULL, "
                       " cod_fisc char(18), "
                       " part_iva char(11), "
                       " fax char(15), "
                       " PRIMARY KEY (id))"
                       "ENGINE = MyISAM"
                       );

            textEdit->append(tr("Creazione tabella clienti effettuata..."));


            query.exec("CREATE TABLE "+db_lylibrary+".articoli "
                       "(cod_articolo INTEGER NOT NULL , "
                       "codbarre char(20) NULL, "
                       "nome_articolo varchar(200) NOT NULL, "
                       "descrizione char(150) NOT NULL, "
                       "autore varchar(30) NOT NULL, "
                       "lingua varchar(30) NOT NULL, "
                       "categ varchar(200) NOT NULL, "
                       "scaffale varchar(30) NOT NULL, "
                       "quantita decimal(10,2) NOT NULL, "
                       "infoeditore varchar(200) NOT NULL, "
                       "image varchar(300), "
                       "PRIMARY KEY(cod_articolo))"
                       "ENGINE = MyISAM"
                       );

            textEdit->append(tr("Creazione tabella libri effettuata..."));


            query.exec("CREATE TABLE "+db_lylibrary+".prodotti_dvd "
                       "(cod_prodotto INTEGER NOT NULL , "
                       "codbarre char(20) NULL, "
                       "nome_prodotto varchar(200) NOT NULL, "
                       "descrizione char(150) NOT NULL, "
                       "autore varchar(30) NOT NULL, "
                       "lingua varchar(30) NOT NULL, "
                       "categ varchar(200) NOT NULL, "
                       "scaffale varchar(30) NOT NULL, "
                       "quantita decimal(10,2) NOT NULL, "
                       "infoeditore varchar(200) NOT NULL, "
                       "image varchar(300), "
                       "PRIMARY KEY(cod_prodotto))"
                       "ENGINE = MyISAM"
                       );

            textEdit->append(tr("Creazione tabella lista prodotti_dvd effettuata..."));


            query.exec("CREATE TABLE "+db_lylibrary+".categoria "
                       "(categ INT NOT NULL , "
                       "tipo_categ varchar(40) NOT NULL, "
                       "PRIMARY KEY(categ))"
                       "ENGINE = MyISAM"
                       );

            textEdit->append(tr("Creazione tabella categoria effettuata..."));

            query.exec("INSERT INTO categoria VALUES('1', 'Architettura')");

            query.exec("INSERT INTO categoria VALUES('2', 'Design')");

            query.exec("INSERT INTO categoria VALUES('3', 'Musica')");

            query.exec("INSERT INTO categoria VALUES('4', 'Storia dell'' arte')");

            query.exec("INSERT INTO categoria VALUES('5', 'Cinema e televisione')");

            query.exec("INSERT INTO categoria VALUES('6', 'Fotografia')");

            query.exec("INSERT INTO categoria VALUES('7', 'Pittura')");

            query.exec("INSERT INTO categoria VALUES('8', 'Teatro')");

            query.exec("INSERT INTO categoria VALUES('9', 'Danza')");

            query.exec("INSERT INTO categoria VALUES('10', 'Musei')");

            query.exec("INSERT INTO categoria VALUES('11', 'Scultura')");

            query.exec("INSERT INTO categoria VALUES('12', 'Economia')");

            query.exec("INSERT INTO categoria VALUES('13', 'Industria e studi di settore')");

            query.exec("INSERT INTO categoria VALUES('14', 'Diritto')");

            query.exec("INSERT INTO categoria VALUES('15', 'Management')");

            query.exec("INSERT INTO categoria VALUES('16', 'Finanza e contabilit√ ')");

            query.exec("INSERT INTO categoria VALUES('17', 'Certificazione informartica')");

            query.exec("INSERT INTO categoria VALUES('18', 'Hardware')");

            query.exec("INSERT INTO categoria VALUES('19', 'Reti e network')");

            query.exec("INSERT INTO categoria VALUES('20', 'Sistemi operativi')");

            query.exec("INSERT INTO categoria VALUES('21', 'Database')");

            query.exec("INSERT INTO categoria VALUES('22', 'Internet & web')");

            query.exec("INSERT INTO categoria VALUES('23', 'Scienza dei calcolatori')");

            query.exec("INSERT INTO categoria VALUES('24', 'Software per ufficio')");

            query.exec("INSERT INTO categoria VALUES('25', 'Grafica e multimedia')");

            query.exec("INSERT INTO categoria VALUES('26', 'Programmazione')");

            query.exec("INSERT INTO categoria VALUES('27', 'Sicurezza informatica')");

            query.exec("INSERT INTO categoria VALUES('28', 'Animali')");

            query.exec("INSERT INTO categoria VALUES('29', 'Giochi e quiz')");

            query.exec("INSERT INTO categoria VALUES('30', 'Modellismo')");

            query.exec("INSERT INTO categoria VALUES('31', 'Videogiochi')");

            query.exec("INSERT INTO categoria VALUES('32', 'Collezionismo')");

            query.exec("INSERT INTO categoria VALUES('33', 'Cucina')");

            query.exec("INSERT INTO categoria VALUES('34', 'Trasporti')");

            query.exec("INSERT INTO categoria VALUES('35', 'Giardinaggio')");

            query.exec("INSERT INTO categoria VALUES('36', 'Video')");

            query.exec("INSERT INTO categoria VALUES('37', 'Antologie')");

            query.exec("INSERT INTO categoria VALUES('38', 'Saggistica')");

            query.exec("INSERT INTO categoria VALUES('39', 'Letteratura teatrale')");

            query.exec("INSERT INTO categoria VALUES('40', 'Poesia')");

            query.exec("INSERT INTO categoria VALUES('41', 'Libri illustrati')");

            query.exec("INSERT INTO categoria VALUES('42', 'Narrativa')");

            query.exec("INSERT INTO categoria VALUES('43', 'Classici')");

            query.exec("INSERT INTO categoria VALUES('44', 'Fiabe')");

            query.exec("INSERT INTO categoria VALUES('45', 'Narrativa storica')");

            query.exec("INSERT INTO categoria VALUES('46', 'Poesia')");

            query.exec("INSERT INTO categoria VALUES('47', 'Antologia')");

            query.exec("INSERT INTO categoria VALUES('48', 'Fantascienza')");

            query.exec("INSERT INTO categoria VALUES('49', 'Giallo & thriller')");

            query.exec("INSERT INTO categoria VALUES('50', 'Racconti')");

            query.exec("INSERT INTO categoria VALUES('51', 'Primo apprendimento')");

            query.exec("INSERT INTO categoria VALUES('52', 'Avventura')");

            query.exec("INSERT INTO categoria VALUES('53', 'Fantasy')");

            query.exec("INSERT INTO categoria VALUES('54', 'Horror')");

            query.exec("INSERT INTO categoria VALUES('55', 'Grammatica')");

            query.exec("INSERT INTO categoria VALUES('56', 'Linguistica')");

            query.exec("INSERT INTO categoria VALUES('57', 'Dizionari')");

            query.exec("INSERT INTO categoria VALUES('58', 'Romanzi rosa')");

            query.exec("INSERT INTO categoria VALUES('59', 'Miti, saghe e leggende')");

            query.exec("INSERT INTO categoria VALUES('60', 'Storia')");

            query.exec("INSERT INTO categoria VALUES('61', 'Italiano')");

            query.exec("INSERT INTO categoria VALUES('62', 'Matematica')");

            qDebug() << query.lastError().text();
            textEdit->append(tr("Inserimento nella tabella categoria effettuata..."));

            query.exec("CREATE TABLE "+db_lylibrary+".biblioteca "
                       "(id INTEGER NOT NULL , "
                       "cliente varchar(300) NOT NULL, "
                       "libro varchar(300) NOT NULL, "
                       "data_prestito DATE NOT NULL, "
                       "ora_prestito char(20) NOT NULL, "
                       "data_rientro DATE NOT NULL, "
                       "ora_rientro char(20) NOT NULL, "
                       "PRIMARY KEY(id))"
                       "ENGINE = MyISAM"
                       );

            textEdit->append(tr("Creazione tabella biblioteca effettuata..."));

            query.exec("CREATE TABLE "+db_lylibrary+".prestiti "
                       "(id INTEGER NOT NULL , "
                       "cliente varchar(300) NOT NULL, "
                       "libro varchar(300) NOT NULL, "
                       "data_prestito DATE NOT NULL, "
                       "data_rientro DATE NOT NULL, "
                       "rientro varchar(30) DEFAULT 'Non rientrato', "
                       "PRIMARY KEY(id))"
                       "ENGINE = MyISAM"
                       );

            query.exec("CREATE TABLE "+db_lylibrary+".biblioteca_dvd "
                       "(id INTEGER NOT NULL , "
                       "cliente varchar(300) NOT NULL, "
                       "dvd varchar(300) NOT NULL, "
                       "data_prestito DATE NOT NULL, "
                       "ora_prestito char(20) NOT NULL, "
                       "data_rientro DATE NOT NULL, "
                       "ora_rientro char(20) NOT NULL, "
                       "PRIMARY KEY(id))"
                       "ENGINE = MyISAM"
                       );

            textEdit->append(tr("Creazione tabella biblioteca_dvd effettuata..."));

            query.exec("CREATE TABLE "+db_lylibrary+".prestiti_dvd "
                       "(id INTEGER NOT NULL , "
                       "cliente varchar(300) NOT NULL, "
                       "dvd varchar(300) NOT NULL, "
                       "data_prestito DATE NOT NULL, "
                       "data_rientro DATE NOT NULL, "
                       "rientro varchar(30) DEFAULT 'Non rientrato', "
                       "PRIMARY KEY(id))"
                       "ENGINE = MyISAM"
                       );
            textEdit->append(tr("Creazione tabella prestiti_dvd effettuata..."));


            query.exec("CREATE TABLE "+db_lylibrary+".causali "
                                   "(id INTEGER NOT NULL, "
                                   "nome_causale char(40) NOT NULL, "
                                   "PRIMARY KEY(id))"
                       "ENGINE = MyISAM"
                                   );
            textEdit->append(tr("Creazione tabella causali"));

            query.exec("insert into causali values ('1','C/LAVORAZIONE')");

            query.exec("insert into causali values ('2','C/RIPARAZIONE')");

            query.exec("insert into causali values ('3','C/VISIONE')");

            query.exec("insert into causali values ('4','CESSIONE')");

            query.exec("insert into causali values ('5','CONSEGNA C/TERZI')");

            query.exec("insert into causali values ('6','LAVORAZIONE')");

            query.exec("insert into causali values ('7','OMAGGIO')");

            query.exec("insert into causali values ('8','RESO')");

            query.exec("insert into causali values ('9','RESO C/LAVORAZIONE')");

            query.exec("insert into causali values ('10','RESO C/RIPARAZIONE')");

            query.exec("insert into causali values ('11','RESO C/VISIONE')");

            query.exec("insert into causali values ('12','RESO SCARTO INUTILIZZABILE')");

            query.exec("insert into causali values ('13','RIPARAZIONE')");

            query.exec("insert into causali values ('14','TENTATA VENDITA')");

            query.exec("insert into causali values ('15', 'VENDITA' )");

            textEdit->append(tr("Inserimento tabella causali"));

            query.exec("CREATE TABLE "+db_lylibrary+".pagam "
                                   "(codice varchar(50) NOT NULL, "
                                   "descrizione varchar(250) NOT NULL, "
                                   "note_fattura char(40) NOT NULL, "
                                   "PRIMARY KEY(codice))"
                       "ENGINE = MyISAM");
            textEdit->append(tr("Creazione tabella pagamenti"));

            query.exec("insert into pagam values ('R.D.','Rimessa diretta','')");

            query.exec("insert into pagam values ('BONIFICO BANCARIO','BONIFICO BANCARIO GENERICO','')");

            query.exec("insert into pagam values ('BONIFICO BANCARIO (A)','BONIFICO BANCARIO (A)', 'Coordinate Bancarie per effettuare bonifico a ns. favore IBAN:')");

            query.exec("insert into pagam values ('BONIFICO BANCARIO (B)','BONIFICO BANCARIO (B)', 'Coordinate Bancarie per effettuare bonifico a ns. favore IBAN:')");

            query.exec("insert into pagam values ('RB 120','Ricevuta Bancaria 120 gg','')");

            query.exec("insert into pagam values ('RB 120 FM','Ricevuta Bancaria 120 gg fine mese','')");

            query.exec("insert into pagam values ('RB 150','Ricevuta Bancaria 150 gg','')");

            query.exec("insert into pagam values ('RB 150 FM','Ricevuta Bancaria 150 gg fine mese','')");

            query.exec("insert into pagam values ('RB 180','Ricevuta Bancaria 180 gg','')");

            query.exec("insert into pagam values ('RB 180 FM','Ricevuta Bancaria 150 gg fine mese','')");

            query.exec("insert into pagam values ('RB 30','Ricevuta Bancaria 30 gg','')");

            query.exec("insert into pagam values ('RB 30 FM','Ricevuta Bancaria 30 gg fine mese','')");

            query.exec("insert into pagam values ('RB 30/60','Ricevuta Bancaria 30/60 gg','')");

            query.exec("insert into pagam values ('RB 30/60 FM','Ricevuta Bancaria 30/60 gg fine mese','')");

            query.exec("insert into pagam values ('RB 30/60/90','Ricevuta Bancaria 30/60/90 gg','')");

            query.exec("insert into pagam values ('RB 30/60/90 FM','Ricevuta Bancaria 30/60/90 gg fine mese','')");

            query.exec("insert into pagam values ('RB 60','Ricevuta Bancaria 60 gg','')");

            query.exec("insert into pagam values ('RB 60 FM','Ricevuta Bancaria 60 gg fine mese','')");

            query.exec("insert into pagam values ('RB 60/90','Ricevuta Bancaria 60/90 gg','')");

            query.exec("insert into pagam values ('RB 60/90 FM','Ricevuta Bancaria 60/90 gg fine mese','')");

            query.exec("insert into pagam values ('RB 90','Ricevuta Bancaria 90 gg','')");

            query.exec("insert into pagam values ('RB 90 FM','Ricevuta Bancaria 90 gg fine mese','')");

            textEdit->append(tr("Inserimento tabella pagamenti"));

            query.exec("CREATE TABLE "+db_lylibrary+".aliquota "
                       "(id INT NOT NULL,"
                       "iva char(40) NOT NULL, "
                       "descrizione char(40) NOT NULL, "
                       "PRIMARY KEY(id))"
                       "ENGINE = MyISAM"
                       );
            textEdit->append(tr("Creazione tabella aliquota iva"));

            query.exec("INSERT INTO aliquota VALUES('1', '21', 'Iva del 21%')");
            query.exec("INSERT INTO aliquota VALUES('2', '10', 'Iva del 10%')");
            query.exec("INSERT INTO aliquota VALUES('3', '4', 'Iva del 4%')");
            query.exec("INSERT INTO aliquota VALUES('4', 'Esente IVA art.15', 'Esente IVA art. 15')");
            query.exec("INSERT INTO aliquota VALUES('5', 'Non imponibile art. 2', 'Non imponibile art. 2')");
            query.exec("INSERT INTO aliquota VALUES('6', 'Non imponibile art.41 D.L 513', 'Non imponibile art.41 D.L 513')");
            query.exec("INSERT INTO aliquota VALUES('7', 'Esente IVA Art. 1 Fin.2008', 'Esente IVA Art. 1 Fin.2008')");
            query.exec("INSERT INTO aliquota VALUES('8', 'Non imponibile art.8', 'Non imponibile art.8')");

            textEdit->append(tr("Inserimento tabella aliquota iva"));

        query.exec("CREATE TABLE "+db_lylibrary+".azienda "
                       "(nome_azienda char(20) NOT NULL, "
                       "partita_iva char(30) NOT NULL, "
                       "cod_fisc char(40) NOT NULL, "
                       "indirizzo char(20) NOT NULL, "
                       "cap char(5) NOT NULL, "
                       "localita char(20) NOT NULL, "
                       "prov char(2) NOT NULL, "
                       "telefono char(20) NOT NULL, "
                       "fax char(20) NOT NULL, "
                       "sito char(40) NOT NULL, "
                       "email char(35) NOT NULL, "
                       "PRIMARY KEY(nome_azienda))"
                        "ENGINE = MyISAM"
                       );

        textEdit->append(tr("Creazione tabella azienda"));

        query.exec("CREATE TABLE "+db_lylibrary+".fornitori "
                       "(id int NOT NULL, "
                       "ragione_sociale char(100) NOT NULL, "
                       "part_iva char(12) NOT NULL, "
                       "cod_fiscale char(16) NOT NULL, "
                       "indirizzo char(20) NOT NULL, "
                       "cap char(20) NOT NULL, "
                       "localita char(30) NOT NULL, "
                       "prov char(30) NOT NULL, "
                       "telefono char(20) NOT NULL, "
                       "fax char(20) NOT NULL, "
                       "email char(30) NOT NULL, "
                       "sito char(30) NOT NULL, "
                       "PRIMARY KEY(id))"
                       "ENGINE = MyISAM"
                       );

           textEdit->append(tr("Creazione tabella fornitori"));

            query.exec("CREATE TABLE "+db_lylibrary+".banca "
                       "(id INT NOT NULL, "
                       "iban char(40) NOT NULL, "
                       "nome_banca char(40) NOT NULL, "
                       "cap char(5) NOT NULL, "
                       "indirizzo char(30) NOT NULL, "
                       "comune char(40) NOT NULL, "
                       "PRIMARY KEY(iban))"
                       "ENGINE = MyISAM"
                       );

            textEdit->append(tr("Creazione tabella banca"));

            query.exec("CREATE TABLE "+db_lylibrary+".porto "
                       "(id INT NOT NULL, "
                       "tipo_di_porto varchar(150) NOT NULL, "
                       "PRIMARY KEY(id))"
                       "ENGINE = MyISAM"
                       );
            textEdit->append(tr("Creazione tabella porto"));

            query.exec("INSERT INTO porto VALUES( '1', 'Porto franco')");
            query.exec("INSERT INTO porto VALUES( '2', 'Porto assegnato')");

            textEdit->append(tr("Inserimento tabella porto"));

            query.exec("CREATE TABLE "+db_lylibrary+" .fatture_acq "
                       "(id INTEGER UNSIGNED NOT NULL, "
                       "data DATE NOT NULL, "
                       "fornitore char(50) NOT NULL, "
                       "tipo_fattura varchar(100) NOT NULL, "
                       "totale decimal(10,2) NOT NULL default 0.00, "
                       "PRIMARY KEY(id)) "
                       "ENGINE = MyISAM");
            textEdit->append(tr("Creazione tabella fatture acquisto"));

            query.exec("CREATE TABLE "+db_lylibrary+" .fatture_acq_righe "
                       "(id INTEGER UNSIGNED NOT NULL, "
                       "data DATE NOT NULL, "
                       "fornitore char(200) NOT NULL, "
                       "sconto_1 decimal(10,2) NOT NULL, "
                       "sconto_2 decimal(10,2) NOT NULL, "
                       "sconto_3 decimal(10,2) NOT NULL, "
                       "spese_trasporto decimal(10,2) NOT NULL, "
                       "spese_incasso decimal(10,2) NOT NULL, "
                       "tipo_pagamento char(200) NOT NULL, "
                       "numero_fattura char(200) NOT NULL, "
                       "data_fattura DATE NOT NULL, "
                       "note_fattura char(40) NOT NULL, "
                       "prezzo_unit decimal(10,2) NOT NULL,"
                       "prezzo_s_iva decimal(10,2) NOT NULL, "
                       "iva decimal(10,2) NOT NULL, "
                       "totale decimal(10,2) NOT NULL, "
                       "PRIMARY KEY(id)) "
                       "ENGINE = MyISAM");
            textEdit->append(tr("Creazione tabella fatture acquisto righe"));

            query.exec("CREATE TABLE "+db_lylibrary+".fatture_righe_acq_art "
                       "(id INTEGER UNSIGNED NOT NULL, "
                       "cod_art INTEGER NOT NULL , "
                       "codbarre char(20) NULL, "
                       "nome_articolo varchar(200) NOT NULL, "
                       "descrizione char(150) NOT NULL, "
                       "unita char(5) NOT NULL, "
                       "prezzo_unit decimal(10,2) NOT NULL,"
                       "prezzo_s_iva decimal(10,2) NOT NULL, "
                       "prezzo_c_iva decimal(10,2) NOT NULL, "
                       "iva decimal(10,2) NOT NULL, "
                       "quantita decimal(10,2) NOT NULL, "
                       "sconto decimal(10,2) NOT NULL, "
                       "totale decimal(10,2) NOT NULL, "
                       "PRIMARY KEY(id))"
                       "ENGINE = MyISAM");
            textEdit->append(tr("Creazione tabella fatture acquisto righe articoli"));

            query.exec("CREATE TABLE "+db_lylibrary+" .fattura_vendita "
                       "(id INTEGER UNSIGNED NOT NULL, "
                       "data DATE NOT NULL, "
                       "fornitore char(50) NOT NULL, "
                       "tipo_fattura varchar(100) NOT NULL, "
                       "totale decimal(10,2) NOT NULL, "
                       "PRIMARY KEY(id)) "
                       "ENGINE = MyISAM");
            textEdit->append(tr("Creazione tabella fatture di vendita"));

            query.exec("CREATE TABLE "+db_lylibrary+" .fatture_vendita_righe "
                       "(id INTEGER UNSIGNED NOT NULL, "
                       "data DATE NOT NULL, "
                       "fornitore char(200) NOT NULL, "
                       "tipo_cliente char(20) NOT NULL, "
                       "sconto_1 decimal(10,2) NOT NULL, "
                       "sconto_2 decimal(10,2) NOT NULL, "
                       "sconto_3 decimal(10,2) NOT NULL, "
                       "spese_trasporto decimal(10,2) NOT NULL, "
                       "spese_incasso decimal(10,2) NOT NULL, "
                       "annotazioni char(120) NOT NULL, "
                       "vs_ordine char(30) NOT NULL, "
                       "tipo_pagamento char(200) NOT NULL, "
                       "note_pagamento char(200) NOT NULL, "
                       "rag_soc char(100) NOT NULL, "
                       "indirizzo char(100) NULL, "
                       "cap char(100) NULL, "
                       "localita char(100) NULL, "
                       "prov char(100) NULL, "
                       "telefono char(200) NULL, "
                       "fax char(200) NULL, "
                       "piva_cfis char(200) NULL, "
                       "banca char(100) NOT NULL,  "
                       "iban char(100) NULL, "
                       "agente char(50) NOT NULL, "
                       "provvigione decimal(10,2) NOT NULL, "
                       "prezzo_s_iva decimal(10,2) NOT NULL, "
                       "iva decimal(10,2) NOT NULL, "
                       "totale decimal(10,2) NOT NULL, "
                       "scadenza_1 DATE NULL, "
                       "scadenza_2 DATE NULL, "
                       "scadenza_3 DATE NULL, "
                       "importo_1 decimal(10,2) NULL, "
                       "importo_2 decimal(10,2) NULL, "
                       "importo_3 decimal(10,2) NULL, "
                       "PRIMARY KEY(id)) "
                       "ENGINE = MyISAM");
            textEdit->append(tr("Creazione tabella fatture vendita righe"));

            query.exec("CREATE TABLE "+db_lylibrary+".fatture_righe_vendita_art "
                       "(id INTEGER UNSIGNED NOT NULL, "
                       "cod_art INTEGER NOT NULL , "
                       "codbarre char(20) NULL, "
                       "nome_articolo varchar(200) NOT NULL, "
                       "descrizione char(150) NOT NULL, "
                       "unita char(5) NOT NULL, "
                       "prezzo_unit decimal(10,2) NOT NULL,"
                       "prezzo_s_iva decimal(10,2) NOT NULL, "
                       "prezzo_c_iva decimal(10,2) NOT NULL, "
                       "iva decimal(10,2) NOT NULL, "
                       "quantita decimal(10,2) NOT NULL, "
                       "sconto decimal(10,2) NOT NULL, "
                       "totale decimal(10,2) NOT NULL, "
                       "PRIMARY KEY(id))"
                       "ENGINE = MyISAM");
            textEdit->append(tr("Creazione tabella fatture acquisto righe articoli"));

            query.exec("CREATE TABLE "+db_lylibrary+" .fattura_vendita_pr_dig "
                       "(id INTEGER UNSIGNED NOT NULL, "
                       "data DATE NOT NULL, "
                       "fornitore char(50) NOT NULL, "
                       "tipo_fattura varchar(100) NOT NULL, "
                       "totale decimal(10,2) NOT NULL, "
                       "PRIMARY KEY(id)) "
                       "ENGINE = MyISAM");
            textEdit->append(tr("Creazione tabella fatture di vendita"));

            query.exec("CREATE TABLE "+db_lylibrary+" .fatture_vendita_righe_dig "
                      "(id INTEGER UNSIGNED NOT NULL, "
                       "data DATE NOT NULL, "
                       "fornitore char(200) NOT NULL, "
                       "tipo_cliente char(20) NOT NULL, "
                       "sconto_1 decimal(10,2) NOT NULL, "
                       "sconto_2 decimal(10,2) NOT NULL, "
                       "sconto_3 decimal(10,2) NOT NULL, "
                       "spese_trasporto decimal(10,2) NOT NULL, "
                       "spese_incasso decimal(10,2) NOT NULL, "
                       "annotazioni char(120) NOT NULL, "
                       "vs_ordine char(30) NOT NULL, "
                       "tipo_pagamento char(200) NOT NULL, "
                       "note_pagamento char(200) NOT NULL, "
                       "rag_soc char(100) NOT NULL, "
                       "indirizzo char(100) NULL, "
                       "cap char(100) NULL, "
                       "localita char(100) NULL, "
                       "prov char(100) NULL, "
                       "telefono char(200) NULL, "
                       "fax char(200) NULL, "
                       "piva_cfis char(200) NULL, "
                       "banca char(100) NOT NULL,  "
                       "iban char(100) NULL, "
                       "agente char(50) NOT NULL, "
                       "provvigione decimal(10,2) NOT NULL, "
                       "prezzo_s_iva decimal(10,2) NOT NULL, "
                       "iva decimal(10,2) NOT NULL, "
                       "totale decimal(10,2) NOT NULL, "
                       "scadenza_1 DATE NULL, "
                       "scadenza_2 DATE NULL, "
                       "scadenza_3 DATE NULL, "
                       "importo_1 decimal(10,2) NULL, "
                       "importo_2 decimal(10,2) NULL, "
                       "importo_3 decimal(10,2) NULL, "
                       "PRIMARY KEY(id)) "
                       "ENGINE = MyISAM");
            textEdit->append(tr("Creazione tabella fatture vendita righe prodotti gitali"));

            query.exec("CREATE TABLE "+db_lylibrary+".fatture_righe_vendita_prod_dig "
                       "(id INTEGER UNSIGNED NOT NULL, "
                       "cod_art INTEGER NOT NULL , "
                       "codbarre char(20) NULL, "
                       "nome_articolo varchar(200) NOT NULL, "
                       "descrizione char(150) NOT NULL, "
                       "unita char(5) NOT NULL, "
                       "prezzo_unit decimal(10,2) NOT NULL,"
                       "prezzo_s_iva decimal(10,2) NOT NULL, "
                       "prezzo_c_iva decimal(10,2) NOT NULL, "
                       "iva decimal(10,2) NOT NULL, "
                       "quantita decimal(10,2) NOT NULL, "
                       "sconto decimal(10,2) NOT NULL, "
                       "totale decimal(10,2) NOT NULL, "
                       "PRIMARY KEY(id))"
                       "ENGINE = MyISAM");
            textEdit->append(tr("Creazione tabella fatture vendita righe prodotti digitali"));

            query.exec("CREATE TABLE "+db_lylibrary+".vettori "
                       "(id INTEGER NOT NULL, "
                       "vett varchar(200) NOT NULL, "
                       "PRIMARY KEY(id))"
                       "ENGINE = MyISAM");
            textEdit->append(tr("Creazione tabella vettori"));

            query.exec("CREATE TABLE "+db_lylibrary+".misura "
                       "(id INT NOT NULL, "
                       "unita char(3) NOT NULL,"
                       "PRIMARY KEY(id))"
                       "ENGINE = MyISAM"
                       );
            textEdit->append(tr("Creazione tabella misura"));

            query.exec("INSERT INTO misura VALUES( '1', 'qt' )");
            query.exec("INSERT INTO misura VALUES( '2', 'mt' )");
            query.exec("INSERT INTO misura VALUES( '3', 'kg' )");
            query.exec("INSERT INTO misura VALUES( '4', 'Lt' )");
            textEdit->append(tr("Inserimento tabella misura"));

            query.exec("CREATE TABLE "+db_lylibrary+".scarico_libri "
                       "(cod_articolo INTEGER NOT NULL , "
                       "codbarre char(20) NULL, "
                       "nome_articolo varchar(200) NOT NULL, "
                       "descrizione char(150) NOT NULL, "
                       "autore varchar(30) NOT NULL, "
                       "lingua varchar(30) NOT NULL, "
                       "categ varchar(200) NOT NULL, "
                       "scaffale varchar(30) NOT NULL, "
                       "quantita decimal(10,2) NOT NULL, "
                       "pr_unit decimal(10,2) NOT NULL, "
                       "pr_s_iva decimal(10,2) NOT NULL, "
                       "pr_c_iva decimal(10,2) NOT NULL, "
                       "totale decimal(10,2) NOT NULL, "
                       "infoeditore varchar(200) NOT NULL, "
                       "image varchar(300), "
                       "PRIMARY KEY(cod_articolo))"
                       "ENGINE = MyISAM"
                       );
            textEdit->append(tr("Creazione tabella scarico libri"));

            query.exec("CREATE TABLE "+db_lylibrary+".scarico_prod_dig "
                       "(cod_articolo INTEGER NOT NULL , "
                       "codbarre char(20) NULL, "
                       "nome_articolo varchar(200) NOT NULL, "
                       "descrizione char(150) NOT NULL, "
                       "autore varchar(30) NOT NULL, "
                       "lingua varchar(30) NOT NULL, "
                       "categ varchar(200) NOT NULL, "
                       "scaffale varchar(30) NOT NULL, "
                       "quantita decimal(10,2) NOT NULL, "
                       "pr_unit decimal(10,2) NOT NULL, "
                       "pr_s_iva decimal(10,2) NOT NULL, "
                       "pr_c_iva decimal(10,2) NOT NULL, "
                       "totale decimal(10,2) NOT NULL, "
                       "infoeditore varchar(200) NOT NULL, "
                       "image varchar(300), "
                       "PRIMARY KEY(cod_articolo))"
                       "ENGINE = MyISAM"
                       );
            textEdit->append(tr("Creazione tabella scarico prodotti digitali"));

            query.exec("CREATE TABLE "+db_lylibrary+".carico_libri"
                       "(cod_articolo INTEGER NOT NULL , "
                       "codbarre char(20) NULL, "
                       "nome_articolo varchar(200) NOT NULL, "
                       "descrizione char(150) NOT NULL, "
                       "autore varchar(30) NOT NULL, "
                       "lingua varchar(30) NOT NULL, "
                       "categ varchar(200) NOT NULL, "
                       "scaffale varchar(30) NOT NULL, "
                       "quantita decimal(10,2) NOT NULL, "
                       "pr_unit decimal(10,2) NOT NULL, "
                       "pr_s_iva decimal(10,2) NOT NULL, "
                       "pr_c_iva decimal(10,2) NOT NULL, "
                       "totale decimal(10,2) NOT NULL, "
                       "infoeditore varchar(200) NOT NULL, "
                       "image varchar(300), "
                       "PRIMARY KEY(cod_articolo))"
                       "ENGINE = MyISAM"
                       );
            textEdit->append(tr("Creazione tabella carico libri"));

            query.exec("CREATE TABLE "+db_lylibrary+".carico_prod_dig"
                       "(cod_articolo INTEGER NOT NULL , "
                       "codbarre char(20) NULL, "
                       "nome_articolo varchar(200) NOT NULL, "
                       "descrizione char(150) NOT NULL, "
                       "autore varchar(30) NOT NULL, "
                       "lingua varchar(30) NOT NULL, "
                       "categ varchar(200) NOT NULL, "
                       "scaffale varchar(30) NOT NULL, "
                       "quantita decimal(10,2) NOT NULL, "
                       "pr_unit decimal(10,2) NOT NULL, "
                       "pr_s_iva decimal(10,2) NOT NULL, "
                       "pr_c_iva decimal(10,2) NOT NULL, "
                       "totale decimal(10,2) NOT NULL, "
                       "infoeditore varchar(200) NOT NULL, "
                       "image varchar(300), "
                       "PRIMARY KEY(cod_articolo))"
                       "ENGINE = MyISAM"
                       );
            textEdit->append(tr("Creazione tabella carico prodotti digitali"));

            general->setValue("Database/databaselocale","locale");
            general->setValue("Database/password",password->text());
            query.exec("GRANT ALL ON lylibrary.* TO 'lylibrary'@'localhost' IDENTIFIED BY 'lylibrary'");
            textEdit->append(tr("Creazione database locale"));

            textEdit->append(tr("Configurazione database in corso..."));
            db.close();

            if(db.open()){

                p_crea->setEnabled(false);
            }
        }

        else
        {


                        QMessageBox MsgBox1;
                        MsgBox1.setText(QString::fromUtf8("Errore di connessione al DB"));
                        MsgBox1.setInformativeText(QString::fromUtf8("Controllare di aver installato Mysql e di aver creato il DB lylibrary"));
                        MsgBox1.setWindowTitle("LyLibrary");
                        QIcon icon;
                        icon.addFile(QString::fromUtf8(":/images/logo1.png"), QSize(), QIcon::Normal, QIcon::Off);
                        MsgBox1.setWindowIcon(icon);
                        MsgBox1.setIcon(QMessageBox::Warning);
                        MsgBox1.exec();
                        }
}

void connessione::load_style(const QString &sheetName){

        QFile file(":/qss/" + sheetName.toLower() + ".qss");
        file.open(QFile::ReadOnly);
        QString styleSheet = QLatin1String(file.readAll());

        qApp->setStyleSheet(styleSheet);
        qApp->setStyle(styleSheet);
}

void connessione::barra_progresso(){

    if(prog <= 100)
    {
    prog++;
    progressBar->setValue(prog);
    }
    else
    {
    //END CODE SNIPPET HERE
        timer->stop();
        delete timer;
    }

    if(prog == 100){
        textEdit->append(tr("Configurazione database completata con successo..."));
        messaggio();

    }
}

void connessione::messaggio(){
    QMessageBox *box= new QMessageBox(this);
    box->setWindowTitle("Lylibrary");
    box->setText("Database");
    box->setInformativeText("Creazione db strutturata con successo....");
    box->setStandardButtons(QMessageBox::Ok);
    box->setDefaultButton(QMessageBox::Ok);
    int ret = box->exec();
    switch(ret){
    case QMessageBox::Ok:
         //Close
        #ifdef Q_OS_LINUX
        QProcess *proc = new QProcess();
        proc->start("/opt/lylibrary/lylibrary");
        #endif
        #ifdef Q_OS_WIN
        QProcess *proc = new QProcess();
        proc->start("C:\\Lylibrary\\lylibrary.exe");
        #endif
        #ifdef Q_OS_MAC
        QProcess *proc = new QProcess();
        proc->start("/Applications/lylibrary.app");
        #endif
         box->close();
         exit(0);
         break;
    }
}

void connessione::connettidatabase(){


    if(ip_dat->text().length() == 0 || n_dat_lan->text().length() == 0 || user->text().length() == 0 || pwd_lan->text().length() == 0){
        QMessageBox MsgBox2;
        MsgBox2.setText(QString::fromUtf8("Errore di connessione al DB"));
        MsgBox2.setInformativeText(QString::fromUtf8("Verificare che i dati siano corretti"));
        MsgBox2.setWindowTitle("LyLibrary");
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/images/logo1.png"), QSize(), QIcon::Normal, QIcon::Off);
        MsgBox2.setWindowIcon(icon);
        MsgBox2.setIcon(QMessageBox::Warning);
        MsgBox2.exec();
        ip_dat->setStyleSheet("background-color: rgb(249, 22, 5)");
        n_dat_lan->setStyleSheet("background-color: rgb(249, 22, 5)");
        user->setStyleSheet("background-color: rgb(249, 22, 5)");
        pwd_lan->setStyleSheet("background-color: rgb(249, 22, 5)");
    }
    else{
    p_crea->setEnabled(false);
    db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName(ip_dat->text());
    db.setPort(port->value());
    db.setDatabaseName(n_dat_lan->text());
    db.setUserName(user->text());
    db.setPassword(pwd_lan->text());


    general->setValue("DatabaseServer/indirizzoip",ip_dat->text());
    general->setValue("DatabaseServer/portadb",port->value());
    general->setValue("DatabaseServer/nomedb",n_dat_lan->text());
    general->setValue("DatabaseServer/userdb",user->text());
    general->setValue("DatabaseServer/passdb",pwd_lan->text());
    general->setValue("Database/databaselocale","server");

    if (db.open())
    {
        QSqlQuery query1;
        query1.exec("GRANT ALL ON lylibrary.* TO 'lylibrary'@'"+ip_dat->text()+"' IDENTIFIED BY 'lylibrary'");
        messaggio();
    }
    else{
        QMessageBox MsgBox2;
        MsgBox2.setText(QString::fromUtf8("Errore di connessione al DB"));
        MsgBox2.setInformativeText(QString::fromUtf8("Impossibile connettersi al db.Controllare le impostazioni."));
        MsgBox2.setWindowTitle("LyLibrary");
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/images/logo1.png"), QSize(), QIcon::Normal, QIcon::Off);
        MsgBox2.setWindowIcon(icon);
        MsgBox2.setIcon(QMessageBox::Warning);
        MsgBox2.exec();
    }
    }
}

void connessione::confread(){

    ip_dat->setText(general->value("DatabaseServer/indirizzoip").toString());
    port->setValue(general->value("DatabaseServer/portadb").toInt());
    n_dat_lan->setText(general->value("DatabaseServer/nomedb").toString());
    user->setText(general->value("DatabaseServer/userdb").toString());
    pwd_lan->setText(general->value("DatabaseServer/passdb").toString());
}

bool connessione::add_vis_groupbox(bool vis)
{
    if(vis == true){
        groupBox_2->setVisible(false);
        QDialog::setFixedSize(424,371);
    }
    else if(vis == false)
    {
        groupBox_2->setVisible(true);
        QDialog::setFixedSize(848,371);
    }

    return vis;
}

void connessione::closeEvent(QCloseEvent *event){

    event->ignore();

}
