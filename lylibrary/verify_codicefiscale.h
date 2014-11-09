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
#ifndef VERIFY_CODICEFISCALE_H
#define VERIFY_CODICEFISCALE_H

/*!
 * \author 2014 Codelinsoft <info@codelinsoft.it>
 */

#include <QDialog>

/*!
 * Classe "ctype.h".
 * \param #include "ctype.h"
 */
#include "ctype.h"


/*!
 * \brief namespace Ui{}
 * \param class verify_codicefiscale;
 */
namespace Ui {
class verify_codicefiscale;
}

/*!
 * La classe verify_codicefiscale è una finestra di dialogo
 * che eredita le funzioni dalla classe Qt
 * QDialog.
 * \brief <b>class verify_codicefiscale: public QDdialog</b>
 * \param QDialog
 */
class verify_codicefiscale : public QDialog
{
    /*!
     * Macro Qt
     *
     *
     * \brief Q_OBJECT
     */
    Q_OBJECT
    
public:
    /*!
     * Costruttore <b>verify_codicefiscale(QWidget *parent = 0);</b>
     *
     * Questa classe visualizza le informazioni
     * relative alla verifica del codice fiscale.
     *
     *
     * \brief <b>verify_codicefiscale(QWidget *parent = 0);</b>
     * \param parent = 0
     */
    explicit verify_codicefiscale(QWidget *parent = 0);

    //!Distruttore ~verify_codicefiscale();
     /*!
      * Il distruttore ~verify_codicefiscale(); serve a chiudere
      * gli eventuali processi attivi.
      *
      *\param delete ui;
      */
    ~verify_codicefiscale();
    
public slots:
    /*!
     * La funzione verify() serve a lanciare il
     * comando di verifica del codice fiscale.
     *
     *\brief <b>verify();</b>
     */
    void verify();

    /*!
     * La funzione read_codice_fis(QString code)
     * serve a leggere e verificare il codice
     * fiscale.
     *
     *\brief <b>read_codice_fis(QString code);</b>
     *\param QString code;----------------[<b>Il parametro code serve a leggere il codice fiscale e verificarlo.</b>]
     */
    QString read_codice_fis(QString code);

private:
    //! Variabile privata
    /*!
     * Variabile ui; Serve a visualizzare gli elementi
     * realizzati con il designer qt.
     *
     *
     * \brief <b>Ui::verify_codicefiscale *ui;<b/>
     * \param ui;
     */
    Ui::verify_codicefiscale *ui;
};

#endif // VERIFY_CODICEFISCALE_H
