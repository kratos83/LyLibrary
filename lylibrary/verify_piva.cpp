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
#include "verify_piva.h"
#include "ui_verify_piva.h"

verify_piva::verify_piva(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::verify_piva)
{
    ui->setupUi(this);
    setWindowTitle(tr("Controlla la tua partita iva"));
    connect(ui->esci,SIGNAL(clicked()),this,SLOT(close()));
    connect(ui->verifica,SIGNAL(clicked()),this,SLOT(verify()));
    ui->piva->setMaxLength(11);
}

void verify_piva::verify()
{
    read_piva(ui->piva->text());
}

QString verify_piva::read_piva(QString code)
{

    int i;
    int p_pari = 0;
    int p_dispari = 0;
    int num_sup = 0;

    QString testo_ndigit = QString::fromUtf8(tr("Errore: Non hai inserito nessun numero.").replace("'","''").toStdString().c_str());

    QString testo_lung= QString::fromUtf8(tr("La lunghezza della partita IVA non è\n"
                                        "corretta: la partita iva deve essere lunga\n"
                                        "esattamente 11 caratteri numerici").replace("'","''").toStdString().c_str());

    QString testo_p_nvalida = QString::fromUtf8(tr("La partita IVA non è valida:\n"
                                                 "il codice di controllo non corrisponde").replace("'","''").toStdString().c_str());

    QString testo_p_valida = QString::fromUtf8(tr("La partita iva è corretta.").replace("'","''").toStdString().c_str());


    if(code.length() == 0){
        ui->icona->setPixmap(QPixmap(QString::fromUtf8(":/images/dialog-close.png")));
        ui->es_er->setText(testo_ndigit);
    }

    if(code.length() != 11 || code == "00000000000"){
        ui->icona->setPixmap(QPixmap(QString::fromUtf8(":/images/dialog-close.png")));
        ui->es_er->setText(testo_lung);
    }
    else{
        for(i=0; i<code.length();i++){
            if(((i+1)%2)==0){
                int k = toascii(code[i].toLatin1()) - toascii('0');
                p_pari += k;
                if(k>5)
                    num_sup++;
            }
            else
                p_dispari += toascii(code[i].toLatin1())- toascii('0');
        }
    }

    int tot = p_pari + p_dispari + num_sup;
    p_pari = tot % 10;
    if(tot == ((10 -p_pari) %10) ){
        ui->icona->setPixmap(QPixmap(QString::fromUtf8(":/images/dialog-close.png")));
        ui->es_er->setText(testo_p_nvalida);
    }
    else{
        ui->icona->setPixmap(QPixmap(QString::fromUtf8(":/images/dialog-ok-apply.png")));
        ui->es_er->setText(testo_p_valida);
    }
    return code;
}

verify_piva::~verify_piva()
{
    delete ui;
}
