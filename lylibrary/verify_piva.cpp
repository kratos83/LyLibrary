#include "verify_piva.h"
#include "ui_verify_piva.h"

verify_piva::verify_piva(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::verify_piva)
{
    ui->setupUi(this);
    setWindowTitle("Controlla la tua partita iva");
    connect(ui->esci,SIGNAL(clicked()),this,SLOT(close()));
    connect(ui->verifica,SIGNAL(clicked()),this,SLOT(verify()));
    connect(ui->piva,SIGNAL(textEdited(QString)),this,SLOT(read_piva(QString)));
}

void verify_piva::verify()
{
    read_piva(ui->piva->text());
}

QString verify_piva::read_piva(QString code)
{

    int i,c,s;

    QString testo_ndigit = QString::fromUtf8("Errore: Non hai inserito nessun numero.");

    QString testo_lung= QString::fromUtf8("La lunghezza della partita IVA non è\n"
                                        "corretta: la partita iva deve essere lunga\n"
                                        "esattamente 11 caratteri numerici");

    QString testo_car = QString::fromUtf8("La partita IVA contiene caratteri non ammessi.\n"
                                         "Inserisci solo caratteri numerici.");

    QString testo_p_nvalida = QString::fromUtf8("La partita IVA non è valida:\n"
                                                 "il codice di controllo non corrisponde");

    QString testo_p_valida = QString::fromUtf8("La partita iva è corretta.");


    if(code.length() == 0){
        ui->icona->setPixmap(QPixmap(QString::fromUtf8(":/images/dialog-close.png")));
        ui->es_er->setText(testo_ndigit);
    }

    if(code.length() != 11){
        ui->icona->setPixmap(QPixmap(QString::fromUtf8(":/images/dialog-close.png")));
        ui->es_er->setText(testo_lung);
    }

    for(i=0; i< 11; i++)
    {
        if(ui->piva->text().compare(QString::number(i))){
            ui->icona->setPixmap(QPixmap(QString::fromUtf8(":/images/dialog-close.png")));
            ui->es_er->setText(testo_car);
        }
    }

    s=0;
    for(i=0; i<9;i+=2)
        s += code[i].toAscii();
    for(i=1;i<9;i+=2){
        c = 2*(code[i].toAscii());
        if(c >9) c = c-9;{
            s+=c;
        }

    if( ( 10 - s%10 )%10 != code[10].ascii() - '0' ){
        ui->icona->setPixmap(QPixmap(QString::fromUtf8(":/images/dialog-close.png")));
        ui->es_er->setText(testo_p_nvalida);
    }
    else{
        ui->icona->setPixmap(QPixmap(QString::fromUtf8(":/images/dialog-ok-apply.png")));
        ui->es_er->setText(testo_p_valida);
    }

    }

    return code;
}

verify_piva::~verify_piva()
{
    delete ui;
}
