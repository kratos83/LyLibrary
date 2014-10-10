/********************************************************************************
** Form generated from reading UI file 'f_rest.ui'
**
** Created by: Qt User Interface Compiler version 4.8.5
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_F_REST_H
#define UI_F_REST_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_f_rest
{
public:
    QLabel *label_2;
    QLabel *label_3;
    QPushButton *riavvia;
    QPushButton *n_rest;

    void setupUi(QDialog *f_rest)
    {
        if (f_rest->objectName().isEmpty())
            f_rest->setObjectName(QString::fromUtf8("f_rest"));
        f_rest->resize(221, 160);
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/images/exit.png"), QSize(), QIcon::Normal, QIcon::Off);
        f_rest->setWindowIcon(icon);
        label_2 = new QLabel(f_rest);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(0, 80, 231, 26));
        label_3 = new QLabel(f_rest);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setGeometry(QRect(80, 10, 64, 64));
        label_3->setPixmap(QPixmap(QString::fromUtf8(":/images/exit.png")));
        riavvia = new QPushButton(f_rest);
        riavvia->setObjectName(QString::fromUtf8("riavvia"));
        riavvia->setGeometry(QRect(10, 130, 65, 24));
        n_rest = new QPushButton(f_rest);
        n_rest->setObjectName(QString::fromUtf8("n_rest"));
        n_rest->setGeometry(QRect(90, 130, 114, 24));

        retranslateUi(f_rest);

        QMetaObject::connectSlotsByName(f_rest);
    } // setupUi

    void retranslateUi(QDialog *f_rest)
    {
        f_rest->setWindowTitle(QApplication::translate("f_rest", "Riavvia Luxury library", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("f_rest", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'Sans Serif'; font-size:10pt; font-weight:400; font-style:normal;\">\n"
"<table border=\"0\" style=\"-qt-table-type: root; margin-top:4px; margin-bottom:4px; margin-left:4px; margin-right:4px;\">\n"
"<tr>\n"
"<td style=\"border: none;\">\n"
"<p align=\"center\" style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Ubuntu'; font-size:9pt;\">Vuoi riavviare Luxury Library?</span></p></td></tr></table></body></html>", 0, QApplication::UnicodeUTF8));
        label_3->setText(QString());
        riavvia->setText(QApplication::translate("f_rest", "Riavvia", 0, QApplication::UnicodeUTF8));
        n_rest->setText(QApplication::translate("f_rest", "Riavvia pi\303\271 tardi", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class f_rest: public Ui_f_rest {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_F_REST_H
