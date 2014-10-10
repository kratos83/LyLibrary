/********************************************************************************
** Form generated from reading UI file 'apri_doc.ui'
**
** Created: Tue May 14 23:31:36 2013
**      by: Qt User Interface Compiler version 4.8.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_APRI_DOC_H
#define UI_APRI_DOC_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QGridLayout>
#include <QtGui/QHeaderView>
#include <QtWebKit/QWebView>

QT_BEGIN_NAMESPACE

class Ui_apri_doc
{
public:
    QGridLayout *gridLayout;
    QWebView *webView;

    void setupUi(QDialog *apri_doc)
    {
        if (apri_doc->objectName().isEmpty())
            apri_doc->setObjectName(QString::fromUtf8("apri_doc"));
        apri_doc->resize(400, 300);
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/images/pdf.png"), QSize(), QIcon::Normal, QIcon::Off);
        apri_doc->setWindowIcon(icon);
        gridLayout = new QGridLayout(apri_doc);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        webView = new QWebView(apri_doc);
        webView->setObjectName(QString::fromUtf8("webView"));
        webView->setUrl(QUrl(QString::fromUtf8("file:///home/angelo/Progetti/softbiblio/lylibrary/images/documentazione_lylibrary.pdf")));

        gridLayout->addWidget(webView, 0, 0, 1, 1);


        retranslateUi(apri_doc);

        QMetaObject::connectSlotsByName(apri_doc);
    } // setupUi

    void retranslateUi(QDialog *apri_doc)
    {
        apri_doc->setWindowTitle(QApplication::translate("apri_doc", "Documentazione LyLibrary", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class apri_doc: public Ui_apri_doc {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_APRI_DOC_H
