/****************************************************************************
**
** Copyright (C) 2006-2014 Angelo e Calogero Scarna
** Contact: Angelo Scarnà (angelo.scarna@codelinsoft.it)
**          Calogero Scarnà (calogero.scarna@codelinsoft.it)
**          Team Codelinsoft (info@codelinsoft.it)
**
** This file is part of the project CodiceFiscale
**
** LGPLv3 License
**
**  You may use this file under the terms of the LGPLv3 license as follows:
*
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of Codelinsoft and its Subsidiary(-ies) nor the names
**     of its contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
****************************************************************************/
#include "verify_codicefiscale.h"
#include "ui_verify_codicefiscale.h"

verify_codicefiscale::verify_codicefiscale(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::verify_codicefiscale)
{
    ui->setupUi(this);
    setWindowTitle(tr("Verifica il tuo codice fiscale"));
    connect(ui->verifica,SIGNAL(clicked()),this,SLOT(verify()));
    connect(ui->esci,SIGNAL(clicked()),this,SLOT(close()));
    ui->codice_fis->setMaxLength(16);
}

void verify_codicefiscale::verify()
{
    read_codice_fis(ui->codice_fis->text());
}

QString verify_codicefiscale::read_codice_fis(QString code)
{
    int s, i, c;
    int setdisp[] = { 1, 0, 5, 7, 9, 13, 15, 17, 19, 21, 2, 4, 18, 20,
            11, 3, 6, 8, 12, 14, 16, 10, 22, 25, 24, 23 };

    QString testo_ndigit = QString::fromUtf8(tr("Errore: Non hai inserito nessun carattere o numero...").replace("'","''").toStdString().c_str());

    QString testo_lung= QString::fromUtf8(tr("La lunghezza del codice fiscale non è\n"
                                        "corretta: il codice fiscale deve essere lungo\n"
                                        "esattamente 16 caratteri...").replace("'","''").toStdString().c_str());

    QString testo_car = QString::fromUtf8(tr("Il codice fiscale inserito non è corretto.\n"
                                         "Il codice di controllo non corrisponde.").replace("'","''").toStdString().c_str());

    QString testo_p_nvalida = QString::fromUtf8(tr("Il codice fiscale contiene dei caratteri non validi:\n"
                                                 "i soli caratteri validi sono le lettere e le cifre.").replace("'","''").toStdString().c_str());

    QString testo_p_valida = QString::fromUtf8(tr("Il codice fiscale inserito è corretto.").replace("'","''").toStdString().c_str());


    if(code.length() == 0){
        ui->icona->setPixmap(QPixmap(QString::fromUtf8(":/images/dialog-close.png")));
        ui->es_er->setText(testo_ndigit);
    }

    if(code.length() != 16){
        ui->icona->setPixmap(QPixmap(QString::fromUtf8(":/images/dialog-close.png")));
        ui->es_er->setText(testo_lung);
    }

    for( i=0; i<16; i++ ){
        c = toupper( code[i].toLatin1() );
            if( !isdigit(c) && !( 'A'<=c && c<='Z' ) )
                ui->icona->setPixmap(QPixmap(QString::fromUtf8(":/images/dialog-close.png")));
                ui->es_er->setText(testo_p_nvalida);
    }

    s = 0;
    for( i=1; i<=13; i+=2 ){
        c = toupper( code[i].toLatin1() );
           if( isdigit(c) )
                s += c - '0';
            else
                s += c - 'A';
    }

    for( i=0; i<=14; i+=2 ){
        c = toupper( code[i].toLatin1() );
            if( isdigit(c) )  c = c - '0' + 'A';
            s += setdisp[c - 'A'];
    }

    if( s%26 + 'A' != toupper( code[15].toLatin1() ) )
    {
        ui->icona->setPixmap(QPixmap(QString::fromUtf8(":/images/dialog-close.png")));
        ui->es_er->setText(testo_car);
    }
    else{
        ui->icona->setPixmap(QPixmap(QString::fromUtf8(":/images/dialog-ok-apply.png")));
        ui->es_er->setText(testo_p_valida);
    }

    return code;
}

verify_codicefiscale::~verify_codicefiscale()
{
    delete ui;
}
