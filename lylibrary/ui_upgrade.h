/********************************************************************************
** Form generated from reading UI file 'upgrade.ui'
**
** Created by: Qt User Interface Compiler version 4.8.5
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_UPGRADE_H
#define UI_UPGRADE_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QGridLayout>
#include <QtGui/QGroupBox>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QProgressBar>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QTextEdit>

QT_BEGIN_NAMESPACE

class Ui_upgrade
{
public:
    QGridLayout *gridLayout_3;
    QGroupBox *groupBox;
    QGridLayout *gridLayout;
    QPushButton *dw_ps;
    QTextEdit *textEdit;
    QProgressBar *progressBar;
    QLabel *test_agg;
    QLabel *label;
    QGroupBox *groupBox_2;
    QGridLayout *gridLayout_2;
    QSpacerItem *horizontalSpacer;
    QPushButton *inst_agg;
    QPushButton *down_agg;

    void setupUi(QDialog *upgrade)
    {
        if (upgrade->objectName().isEmpty())
            upgrade->setObjectName(QString::fromUtf8("upgrade"));
        upgrade->resize(466, 296);
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/images/system-software-update.png"), QSize(), QIcon::Normal, QIcon::Off);
        upgrade->setWindowIcon(icon);
        gridLayout_3 = new QGridLayout(upgrade);
        gridLayout_3->setObjectName(QString::fromUtf8("gridLayout_3"));
        groupBox = new QGroupBox(upgrade);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        gridLayout = new QGridLayout(groupBox);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        dw_ps = new QPushButton(groupBox);
        dw_ps->setObjectName(QString::fromUtf8("dw_ps"));
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/images/pause.png"), QSize(), QIcon::Normal, QIcon::Off);
        dw_ps->setIcon(icon1);

        gridLayout->addWidget(dw_ps, 1, 1, 1, 1);

        textEdit = new QTextEdit(groupBox);
        textEdit->setObjectName(QString::fromUtf8("textEdit"));
        textEdit->setReadOnly(true);

        gridLayout->addWidget(textEdit, 3, 0, 1, 3);

        progressBar = new QProgressBar(groupBox);
        progressBar->setObjectName(QString::fromUtf8("progressBar"));
        progressBar->setValue(0);

        gridLayout->addWidget(progressBar, 1, 0, 1, 1);

        test_agg = new QLabel(groupBox);
        test_agg->setObjectName(QString::fromUtf8("test_agg"));

        gridLayout->addWidget(test_agg, 2, 0, 1, 1);

        label = new QLabel(groupBox);
        label->setObjectName(QString::fromUtf8("label"));

        gridLayout->addWidget(label, 0, 0, 1, 1);


        gridLayout_3->addWidget(groupBox, 0, 0, 1, 1);

        groupBox_2 = new QGroupBox(upgrade);
        groupBox_2->setObjectName(QString::fromUtf8("groupBox_2"));
        gridLayout_2 = new QGridLayout(groupBox_2);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        horizontalSpacer = new QSpacerItem(54, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_2->addItem(horizontalSpacer, 0, 0, 1, 1);

        inst_agg = new QPushButton(groupBox_2);
        inst_agg->setObjectName(QString::fromUtf8("inst_agg"));
        QIcon icon2;
        icon2.addFile(QString::fromUtf8(":/images/install.png"), QSize(), QIcon::Normal, QIcon::Off);
        inst_agg->setIcon(icon2);

        gridLayout_2->addWidget(inst_agg, 0, 1, 1, 1);

        down_agg = new QPushButton(groupBox_2);
        down_agg->setObjectName(QString::fromUtf8("down_agg"));
        QIcon icon3;
        icon3.addFile(QString::fromUtf8(":/images/upgrade.png"), QSize(), QIcon::Normal, QIcon::Off);
        down_agg->setIcon(icon3);

        gridLayout_2->addWidget(down_agg, 0, 2, 1, 1);

        down_agg->raise();
        inst_agg->raise();

        gridLayout_3->addWidget(groupBox_2, 1, 0, 1, 1);

        QWidget::setTabOrder(dw_ps, textEdit);
        QWidget::setTabOrder(textEdit, inst_agg);
        QWidget::setTabOrder(inst_agg, down_agg);

        retranslateUi(upgrade);

        QMetaObject::connectSlotsByName(upgrade);
    } // setupUi

    void retranslateUi(QDialog *upgrade)
    {
        upgrade->setWindowTitle(QApplication::translate("upgrade", "Aggiornamento software", 0, QApplication::UnicodeUTF8));
        groupBox->setTitle(QString());
#ifndef QT_NO_TOOLTIP
        dw_ps->setToolTip(QApplication::translate("upgrade", "<html><head/><body><p>Annulla download</p></body></html>", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        dw_ps->setText(QString());
        test_agg->setText(QString());
        label->setText(QApplication::translate("upgrade", "Download aggiornamento", 0, QApplication::UnicodeUTF8));
        groupBox_2->setTitle(QString());
#ifndef QT_NO_TOOLTIP
        inst_agg->setToolTip(QApplication::translate("upgrade", "<html><head/><body><p>Installa aggiornamento</p></body></html>", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        inst_agg->setText(QApplication::translate("upgrade", "Installa aggiornamento", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        down_agg->setToolTip(QApplication::translate("upgrade", "<html><head/><body><p>Download aggiornamento</p></body></html>", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        down_agg->setText(QApplication::translate("upgrade", "Downaload aggiornamento", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class upgrade: public Ui_upgrade {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_UPGRADE_H
