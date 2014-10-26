#include "cod_fisc.h"
#include "ui_cod_fisc.h"
#include <QFile>
#include <QTextStream>
#include <QtGui/QMessageBox>
#include <iostream>
#include <QCompleter>
#include <QDebug>
#include "print.h"

cod_fisc::cod_fisc(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::cod_fisc)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose);
    today = ui->calendarWidget->selectedDate();
    connect(ui->actionVisualizza_anteprima,SIGNAL(triggered(bool)),this,SLOT(vis_cod_fisc(bool)));
    connect(ui->actionStampa,SIGNAL(triggered()),this,SLOT(stampa_codice()));
    connect(ui->actionEsci,SIGNAL(triggered()),this,SLOT(on_esci_clicked()));
    ui->actionVisualizza_anteprima->setChecked(true);
    ui->radioButtonM->setChecked(true);
    setWindowTitle("Codice fiscale italiano");
    img = QPixmap(":/images/codicefiscale.png");
    ui->tab_2->setEnabled(false);
    ui->tab->setVisible(true);
    initComuni();
}

void cod_fisc::initComuni(){

    QFile file(":/comuni.txt");

            if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
                    return;

            QTextStream in(&file);
            QString com;
            for (int i = 0; i < 8098; i++)
            {
                    QString line = in.readLine();
                    com  = line.section(':', 0, 0);
                    ui->comboBoxComune->addItem(com);
            }
}

void cod_fisc::on_pushButtonCalcola_clicked(){

    // acquisizione dati
            cognome = ui->lineEditCognome->displayText().toUpper();
            nome = ui->lineEditNome->displayText().toUpper();
            sesso = (ui->radioButtonM->isChecked() == true);
            comune = ui->comboBoxComune->currentText().toUpper();

            QDate date = ui->calendarWidget->selectedDate();
            dataGiorno = date.day();
            dataMese = date.month();
            dataAnno = date.year();

            if (!chechInput())
                    return;

            // inizio algoritmi
            calculateCognome();
            calculateNome();
            calculateData();
            calculateComune();
            calculateLetteraControllo();

            merge();
}

void cod_fisc::on_pushButtonCancella_clicked(){

    ui->lineEditCognome->setText("");
    ui->lineEditNome->setText("");
    ui->radioButtonM->setChecked(true);
    ui->comboBoxComune->setCurrentIndex(0);
    ui->calendarWidget->setSelectedDate(today);
    ui->lineEditCodiceFiscale->setText("");
}

void cod_fisc::calculateCognome(){

    block1 = "";

            // calcolo consonanti
            for (int i = 0; i < cognome.length(); i++)
            {
                    QString tmp = QString(cognome[i]);

                    if (checkVocale(tmp) == 0)
                    {
                            block1 += tmp;

                            if (block1.length() == 3)
                                    return;
                    }
            }

            // calcolo eventuali vocali
            for (int i = 0; i < cognome.length(); i++)
            {
                    QString tmp = QString(cognome[i]);

                    if (checkVocale(tmp) == 1)
                    {
                            block1 += tmp;

                            if (block1.length() == 3)
                                    return;
                    }
            }

            // calcolo eventuali X
            if (block1.length() == 1)
                    block1 += "XX";
            else if (block1.length() == 2)
                    block1 += "X";
}

void cod_fisc::calculateNome(){

    block2 = "";
            bool ok = false;

            // calcolo consonanti (test sullo scarto della 2° consonante)
            for (int i = 0; i < nome.length(); i++)
            {
                    QString tmp = QString(nome[i]);

                    if (checkVocale(tmp) == 0)
                    {
                            if ((ok == false) && (block2.length() == 1))
                                    ok = true;
                            else
                                    block2 += tmp;

                            if (block2.length() == 3)
                                    return;
                    }
            }

            // non bastano le consonanti... rifare daccapo
            block2 = "";

            // calcolo consonanti
            for (int i = 0; i < nome.length(); i++)
            {
                    QString tmp = QString(nome[i]);

                    if (checkVocale(tmp) == 0)
                    {
                            block2 += tmp;

                            if (block2.length() == 3)
                                    return;
                    }
            }

            // calcolo eventuali vocali
            for (int i = 0; i < nome.length(); i++)
            {
                    QString tmp = QString(nome[i]);

                    if (checkVocale(tmp) == 1)
                    {
                            block2 += tmp;

                            if (block2.length() == 3)
                                    return;
                    }
            }

            // calcolo eventuali X
            if (block2.length() == 1)
                    block2 += "XX";
            else if (block2.length() == 2)
                    block2 += "X";
}

void cod_fisc::calculateData(){

    // calcolo  dell'anno
            QString anno;
            anno.setNum(dataAnno);
            block3 = anno.right(2);

            // calcolo  del mese
            switch (dataMese)
            {
                    case 1:
                            block3 += "A"; // Gennaio = A
                            break;
                    case 2:
                            block3 += "B"; // Febbraio = B
                            break;
                    case 3:
                            block3 += "C"; // Marzo = C
                            break;
                    case 4:
                            block3 += "D"; // Aprile = D
                            break;
                    case 5:
                            block3 += "E"; // Maggio = E
                            break;
                    case 6:
                            block3 += "H"; // Giugno = H
                            break;
                    case 7:
                            block3 += "L"; // Luglio = L
                            break;
                    case 8:
                            block3 += "M"; // Agosto = M
                            break;
                    case 9:
                            block3 += "P"; // Settembre = P
                            break;
                    case 10:
                            block3 += "R"; // Ottobre = R
                            break;
                    case 11:
                            block3 += "S"; // Novembre = S
                            break;
                    case 12:
                            block3 += "T"; // Dicembre = T
                            break;
            }

            // calcolo  del giorno
            QString giorno;
            if (ui->radioButtonM->isChecked())
            {
                    giorno.setNum(dataGiorno);
                    if (giorno.length() == 1)
                    {
                            QString tmp = giorno;
                            giorno = "0" + tmp;
                    }
            }
            else if (ui->radioButtonF->isChecked())
                    giorno.setNum(dataGiorno+40);

            block3 += giorno;
}

void cod_fisc::calculateComune(){

    int currentIndex = ui->comboBoxComune->currentIndex();

    QFile file(":/comuni.txt");

            if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
                    return;

            QTextStream in(&file);

            for (int i = 0; i < currentIndex; i++)
                    in.readLine();

            QString line = in.readLine();
            QString cod = line.section(':', 1, 1);
            block4 = cod.left(4);
}

void cod_fisc::calculateLetteraControllo(){

    int sum = 0;
            QString CFtmp = block1+block2+block3+block4;

            for (int i = 0; i < 15; i++)
            {
                    if ((i % 2) == 1)
                            sum += getEvenCode(QString(CFtmp[i]));
                    else
                            sum += getOddCode(QString(CFtmp[i]));
            }

            int total = sum % 26;
            block5 = mapNumber(total);
}

void cod_fisc::merge(){

    CF = block1+block2+block3+block4+block5;

            ui->lineEditCodiceFiscale->setText(CF);
}

int cod_fisc::checkVocale(QString v){

    v.toUpper();

            if (v == " ")
                    return -1;

            if ((v == "A") || (v == "E") || (v == "I") || (v == "O") || (v == "U"))
                    return 1;
            return 0;
}

bool cod_fisc::chechInput(){

    QString errors = "";

            if (ui->lineEditCognome->displayText() == "")
                    errors += "Inserisci il cognome\n";
            if (ui->lineEditNome->displayText() == "")
                    errors += "Inserisci il nome\n";
            if ((!ui->radioButtonM->isChecked()) && (!ui->radioButtonF->isChecked()))
                    errors += "Inserisci il sesso\n";

            if (errors != "")
            {
                    errors += "\nFinisci di inserire i dati correttamente";
                    QMessageBox::critical(this, tr("Errore"), errors);
                    return false;
            }

            return true;
}

int cod_fisc::getEvenCode(QString c){

    // calcolo del carattere
            if ((c == "A") || (c == "0"))
                    return 0;
            if ((c == "B") || (c == "1"))
                    return 1;
            if ((c == "C") || (c == "2"))
                    return 2;
            if ((c == "D") || (c == "3"))
                    return 3;
            if ((c == "E") || (c == "4"))
                    return 4;
            if ((c == "F") || (c == "5"))
                    return 5;
            if ((c == "G") || (c == "6"))
                    return 6;
            if ((c == "H") || (c == "7"))
                    return 7;
            if ((c == "I") || (c == "8"))
                    return 8;
            if ((c == "J") || (c == "9"))
                    return 9;
            if (c == "K")
                    return 10;
            if (c == "L")
                    return 11;
            if (c == "M")
                    return 12;
            if (c == "N")
                    return 13;
            if (c == "O")
                    return 14;
            if (c == "P")
                    return 15;
            if (c == "Q")
                    return 16;
            if (c == "R")
                    return 17;
            if (c == "S")
                    return 18;
            if (c == "T")
                    return 19;
            if (c == "U")
                    return 20;
            if (c == "V")
                    return 21;
            if (c == "W")
                    return 22;
            if (c == "X")
                    return 23;
            if (c == "Y")
                    return 24;
            if (c == "Z")
                    return 25;

            return -1;
}

int cod_fisc::getOddCode(QString c){

    // calcolo del carattere
            if ((c == "A") || (c == "0"))
                    return 1;
            if ((c == "B") || (c == "1"))
                    return 0;
            if ((c == "C") || (c == "2"))
                    return 5;
            if ((c == "D") || (c == "3"))
                    return 7;
            if ((c == "E") || (c == "4"))
                    return 9;
            if ((c == "F") || (c == "5"))
                    return 13;
            if ((c == "G") || (c == "6"))
                    return 15;
            if ((c == "H") || (c == "7"))
                    return 17;
            if ((c == "I") || (c == "8"))
                    return 19;
            if ((c == "J") || (c == "9"))
                    return 21;
            if (c == "K")
                    return 2;
            if (c == "L")
                    return 4;
            if (c == "M")
                    return 18;
            if (c == "N")
                    return 20;
            if (c == "O")
                    return 11;
            if (c == "P")
                    return 3;
            if (c == "Q")
                    return 6;
            if (c == "R")
                    return 8;
            if (c == "S")
                    return 12;
            if (c == "T")
                    return 14;
            if (c == "U")
                    return 16;
            if (c == "V")
                    return 10;
            if (c == "W")
                    return 22;
            if (c == "X")
                    return 25;
            if (c == "Y")
                    return 24;
            if (c == "Z")
                    return 23;

            return -1;
}

QString cod_fisc::mapNumber(int n){

    // calcolo del carattere
            if (n == 0)
                    return "A";
            if (n == 1)
                    return "B";
            if (n == 2)
                    return "C";
            if (n == 3)
                    return "D";
            if (n == 4)
                    return "E";
            if (n == 5)
                    return "F";
            if (n == 6)
                    return "G";
            if (n == 7)
                    return "H";
            if (n == 8)
                    return "I";
            if (n == 9)
                    return "J";
            if (n == 10)
                    return "K";
            if (n == 11)
                    return "L";
            if (n == 12)
                    return "M";
            if (n == 13)
                    return "N";
            if (n == 14)
                    return "O";
            if (n == 15)
                    return "P";
            if (n == 16)
                    return "Q";
            if (n == 17)
                    return "R";
            if (n == 18)
                    return "S";
            if (n == 19)
                    return "T";
            if (n == 20)
                    return "U";
            if (n == 21)
                    return "V";
            if (n == 22)
                    return "W";
            if (n == 23)
                    return "X";
            if (n == 24)
                    return "Y";
            if (n == 25)
                    return "Z";

            return "_";
}

cod_fisc::~cod_fisc()
{
    delete ui;
}

bool cod_fisc::vis_cod_fisc(bool click)
{

    if(click == true)
    {
        setFixedSize(846,521);
    }
    else if(click == false)
       {
        setFixedSize(418,521);
       }

    return click;
}

void cod_fisc::paintEvent(QPaintEvent *event)
{
    event = event;
    painter = new QPainter(this);
    painter->drawImage(435,60,img);
    painter->setPen(QPen(Qt::black, 12, Qt::SolidLine, Qt::RoundCap));
    painter->drawText(330,130,480,100,Qt::AlignVCenter | Qt::AlignHCenter | Qt::TextWordWrap,CF);
    painter->drawText(300,155,480,100, Qt::AlignVCenter | Qt::AlignHCenter | Qt::TextWordWrap,ui->lineEditCognome->text());
    painter->drawText(300,180,480,100,Qt::AlignVCenter | Qt::AlignHCenter | Qt::TextWordWrap, ui->lineEditNome->text());
    painter->drawText(330,210,480,100,Qt::AlignVCenter | Qt::AlignHCenter | Qt::TextWordWrap, ui->comboBoxComune->currentText());
    painter->drawText(330,235,480,100,Qt::AlignVCenter | Qt::AlignHCenter | Qt::TextWordWrap,ui->comboBoxComune->currentText());
    painter->drawText(330,260,480,100,Qt::AlignVCenter | Qt::AlignHCenter | Qt::TextWordWrap,ui->calendarWidget->selectedDate().toString(QString("dd/MM/yyyy")));
    if(ui->radioButtonM->isChecked())
    {
        painter->drawText(550,195,480,100,Qt::AlignVCenter | Qt::AlignHCenter | Qt::TextWordWrap,"M");
    }
    else if(ui->radioButtonF->isChecked())
    {
        painter->drawText(550,195,480,100,Qt::AlignVCenter | Qt::AlignHCenter | Qt::TextWordWrap,"F");
    }
    painter->setFont(QFont("Arial",18,50));
    painter->drawText(513,250,480,100,Qt::AlignVCenter | Qt::AlignHCenter | Qt::TextWordWrap,"FAC SIMILE");
    update();
    painter->end();
}

void cod_fisc::stampa_codice()
{
    QPrinter printer(QPrinter::HighResolution);

    QPrintPreviewDialog preview(&printer);
    preview.setWindowFlags(Qt::Window);
    preview.setWindowTitle("Anteprima di stampa.");
    connect(&preview,SIGNAL(paintRequested(QPrinter*)),SLOT(anteprima(QPrinter*)));
    preview.exec();
}

void cod_fisc::anteprima(QPrinter *printer)
{
    Q_UNUSED(printer);
    Stampe *stampa = new Stampe();
    QImage image_800x600 = QImage(":/images/codicefiscale_800x600.png");
    stampa->print_codice(printer,image_800x600,ui->lineEditCognome->text(),ui->lineEditNome->text(),ui->radioButtonM->isChecked(),ui->radioButtonF->isChecked(),ui->comboBoxComune->currentText(),ui->comboBoxComune->currentText(),CF,ui->calendarWidget->selectedDate());
}

void cod_fisc::on_esci_clicked()
{
    close();
}
