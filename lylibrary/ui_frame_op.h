/********************************************************************************
** Form generated from reading UI file 'frame_op.ui'
**
** Created by: Qt User Interface Compiler version 4.8.5
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_FRAME_OP_H
#define UI_FRAME_OP_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QGridLayout>
#include <QtGui/QGroupBox>
#include <QtGui/QHeaderView>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_frame_op
{
public:
    QGridLayout *gridLayout_6;
    QGroupBox *groupBox;
    QGridLayout *gridLayout;
    QPushButton *libri;
    QSpacerItem *horizontalSpacer;
    QPushButton *categoria;
    QPushButton *clienti;
    QGroupBox *groupBox_3;
    QGridLayout *gridLayout_3;
    QPushButton *backup_restore;
    QPushButton *preferenze;
    QPushButton *wizard_avvio;
    QSpacerItem *horizontalSpacer_2;
    QGroupBox *groupBox_2;
    QGridLayout *gridLayout_2;
    QPushButton *cl2;
    QPushButton *scadenze;
    QSpacerItem *horizontalSpacer_4;
    QGroupBox *groupBox_4;
    QGridLayout *gridLayout_4;
    QPushButton *about_luxury;
    QPushButton *about_qt;
    QSpacerItem *horizontalSpacer_6;
    QSpacerItem *horizontalSpacer_9;
    QGroupBox *groupBox_5;
    QGridLayout *gridLayout_5;
    QSpacerItem *horizontalSpacer_7;
    QPushButton *esci_prg;
    QSpacerItem *horizontalSpacer_8;
    QSpacerItem *horizontalSpacer_10;
    QSpacerItem *horizontalSpacer_13;
    QGroupBox *groupBox_6;
    QGridLayout *gridLayout_8;
    QGridLayout *gridLayout_7;
    QSpacerItem *horizontalSpacer_14;
    QSpacerItem *verticalSpacer;
    QSpacerItem *horizontalSpacer_3;
    QSpacerItem *horizontalSpacer_5;
    QSpacerItem *horizontalSpacer_11;
    QSpacerItem *horizontalSpacer_12;

    void setupUi(QWidget *frame_op)
    {
        if (frame_op->objectName().isEmpty())
            frame_op->setObjectName(QString::fromUtf8("frame_op"));
        frame_op->resize(810, 502);
        gridLayout_6 = new QGridLayout(frame_op);
        gridLayout_6->setObjectName(QString::fromUtf8("gridLayout_6"));
        groupBox = new QGroupBox(frame_op);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        gridLayout = new QGridLayout(groupBox);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        libri = new QPushButton(groupBox);
        libri->setObjectName(QString::fromUtf8("libri"));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(151);
        sizePolicy.setVerticalStretch(60);
        sizePolicy.setHeightForWidth(libri->sizePolicy().hasHeightForWidth());
        libri->setSizePolicy(sizePolicy);
        libri->setMinimumSize(QSize(151, 60));
        libri->setMaximumSize(QSize(130, 60));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/images/accessories-dictionary.png"), QSize(), QIcon::Normal, QIcon::Off);
        libri->setIcon(icon);
        libri->setIconSize(QSize(32, 32));

        gridLayout->addWidget(libri, 0, 0, 1, 1);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer, 0, 1, 1, 1);

        categoria = new QPushButton(groupBox);
        categoria->setObjectName(QString::fromUtf8("categoria"));
        QSizePolicy sizePolicy1(QSizePolicy::Minimum, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(6);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(categoria->sizePolicy().hasHeightForWidth());
        categoria->setSizePolicy(sizePolicy1);
        categoria->setMinimumSize(QSize(130, 60));
        categoria->setMaximumSize(QSize(151, 60));
        categoria->setBaseSize(QSize(151, 60));
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/images/documentation.png"), QSize(), QIcon::Normal, QIcon::Off);
        categoria->setIcon(icon1);
        categoria->setIconSize(QSize(32, 32));

        gridLayout->addWidget(categoria, 0, 2, 1, 1);

        clienti = new QPushButton(groupBox);
        clienti->setObjectName(QString::fromUtf8("clienti"));
        sizePolicy1.setHeightForWidth(clienti->sizePolicy().hasHeightForWidth());
        clienti->setSizePolicy(sizePolicy1);
        clienti->setMinimumSize(QSize(151, 60));
        clienti->setMaximumSize(QSize(151, 60));
        clienti->setBaseSize(QSize(151, 60));
        QIcon icon2;
        icon2.addFile(QString::fromUtf8(":/images/user-group-properties.png"), QSize(), QIcon::Normal, QIcon::Off);
        clienti->setIcon(icon2);
        clienti->setIconSize(QSize(32, 32));

        gridLayout->addWidget(clienti, 1, 0, 1, 1);


        gridLayout_6->addWidget(groupBox, 0, 1, 1, 3);

        groupBox_3 = new QGroupBox(frame_op);
        groupBox_3->setObjectName(QString::fromUtf8("groupBox_3"));
        gridLayout_3 = new QGridLayout(groupBox_3);
        gridLayout_3->setObjectName(QString::fromUtf8("gridLayout_3"));
        backup_restore = new QPushButton(groupBox_3);
        backup_restore->setObjectName(QString::fromUtf8("backup_restore"));
        sizePolicy1.setHeightForWidth(backup_restore->sizePolicy().hasHeightForWidth());
        backup_restore->setSizePolicy(sizePolicy1);
        backup_restore->setMinimumSize(QSize(151, 60));
        backup_restore->setMaximumSize(QSize(151, 60));
        backup_restore->setBaseSize(QSize(151, 60));
        QIcon icon3;
        icon3.addFile(QString::fromUtf8(":/images/svn-commit.png"), QSize(), QIcon::Normal, QIcon::Off);
        backup_restore->setIcon(icon3);
        backup_restore->setIconSize(QSize(32, 32));

        gridLayout_3->addWidget(backup_restore, 0, 0, 1, 1);

        preferenze = new QPushButton(groupBox_3);
        preferenze->setObjectName(QString::fromUtf8("preferenze"));
        sizePolicy1.setHeightForWidth(preferenze->sizePolicy().hasHeightForWidth());
        preferenze->setSizePolicy(sizePolicy1);
        preferenze->setMinimumSize(QSize(151, 60));
        preferenze->setMaximumSize(QSize(151, 60));
        preferenze->setBaseSize(QSize(151, 60));
        QIcon icon4;
        icon4.addFile(QString::fromUtf8(":/images/preferences-system.png"), QSize(), QIcon::Normal, QIcon::Off);
        preferenze->setIcon(icon4);
        preferenze->setIconSize(QSize(32, 32));

        gridLayout_3->addWidget(preferenze, 0, 2, 1, 1);

        wizard_avvio = new QPushButton(groupBox_3);
        wizard_avvio->setObjectName(QString::fromUtf8("wizard_avvio"));
        sizePolicy1.setHeightForWidth(wizard_avvio->sizePolicy().hasHeightForWidth());
        wizard_avvio->setSizePolicy(sizePolicy1);
        wizard_avvio->setMinimumSize(QSize(151, 60));
        wizard_avvio->setMaximumSize(QSize(151, 60));
        wizard_avvio->setBaseSize(QSize(151, 60));
        QIcon icon5;
        icon5.addFile(QString::fromUtf8(":/images/system-run.png"), QSize(), QIcon::Normal, QIcon::Off);
        wizard_avvio->setIcon(icon5);
        wizard_avvio->setIconSize(QSize(32, 32));

        gridLayout_3->addWidget(wizard_avvio, 1, 0, 1, 1);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_3->addItem(horizontalSpacer_2, 0, 1, 1, 1);


        gridLayout_6->addWidget(groupBox_3, 0, 4, 1, 3);

        groupBox_2 = new QGroupBox(frame_op);
        groupBox_2->setObjectName(QString::fromUtf8("groupBox_2"));
        gridLayout_2 = new QGridLayout(groupBox_2);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        cl2 = new QPushButton(groupBox_2);
        cl2->setObjectName(QString::fromUtf8("cl2"));
        sizePolicy1.setHeightForWidth(cl2->sizePolicy().hasHeightForWidth());
        cl2->setSizePolicy(sizePolicy1);
        cl2->setMinimumSize(QSize(151, 60));
        cl2->setMaximumSize(QSize(151, 60));
        cl2->setBaseSize(QSize(151, 60));
        cl2->setIcon(icon2);
        cl2->setIconSize(QSize(32, 32));

        gridLayout_2->addWidget(cl2, 0, 0, 1, 1);

        scadenze = new QPushButton(groupBox_2);
        scadenze->setObjectName(QString::fromUtf8("scadenze"));
        sizePolicy1.setHeightForWidth(scadenze->sizePolicy().hasHeightForWidth());
        scadenze->setSizePolicy(sizePolicy1);
        scadenze->setMinimumSize(QSize(151, 60));
        scadenze->setMaximumSize(QSize(151, 60));
        scadenze->setBaseSize(QSize(151, 60));
        scadenze->setIcon(icon2);
        scadenze->setIconSize(QSize(32, 32));

        gridLayout_2->addWidget(scadenze, 0, 2, 1, 1);

        horizontalSpacer_4 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_2->addItem(horizontalSpacer_4, 0, 1, 1, 1);


        gridLayout_6->addWidget(groupBox_2, 1, 1, 1, 3);

        groupBox_4 = new QGroupBox(frame_op);
        groupBox_4->setObjectName(QString::fromUtf8("groupBox_4"));
        gridLayout_4 = new QGridLayout(groupBox_4);
        gridLayout_4->setObjectName(QString::fromUtf8("gridLayout_4"));
        about_luxury = new QPushButton(groupBox_4);
        about_luxury->setObjectName(QString::fromUtf8("about_luxury"));
        sizePolicy1.setHeightForWidth(about_luxury->sizePolicy().hasHeightForWidth());
        about_luxury->setSizePolicy(sizePolicy1);
        about_luxury->setMinimumSize(QSize(151, 60));
        about_luxury->setMaximumSize(QSize(151, 60));
        about_luxury->setBaseSize(QSize(151, 60));
        QIcon icon6;
        icon6.addFile(QString::fromUtf8(":/images/svn_status.png"), QSize(), QIcon::Normal, QIcon::Off);
        about_luxury->setIcon(icon6);
        about_luxury->setIconSize(QSize(32, 32));

        gridLayout_4->addWidget(about_luxury, 0, 0, 1, 1);

        about_qt = new QPushButton(groupBox_4);
        about_qt->setObjectName(QString::fromUtf8("about_qt"));
        sizePolicy1.setHeightForWidth(about_qt->sizePolicy().hasHeightForWidth());
        about_qt->setSizePolicy(sizePolicy1);
        about_qt->setMinimumSize(QSize(151, 60));
        about_qt->setMaximumSize(QSize(151, 60));
        about_qt->setBaseSize(QSize(151, 60));
        about_qt->setIcon(icon6);
        about_qt->setIconSize(QSize(32, 32));

        gridLayout_4->addWidget(about_qt, 0, 2, 1, 1);

        horizontalSpacer_6 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_4->addItem(horizontalSpacer_6, 0, 1, 1, 1);


        gridLayout_6->addWidget(groupBox_4, 1, 4, 1, 3);

        horizontalSpacer_9 = new QSpacerItem(284, 93, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_6->addItem(horizontalSpacer_9, 2, 1, 1, 2);

        groupBox_5 = new QGroupBox(frame_op);
        groupBox_5->setObjectName(QString::fromUtf8("groupBox_5"));
        gridLayout_5 = new QGridLayout(groupBox_5);
        gridLayout_5->setObjectName(QString::fromUtf8("gridLayout_5"));
        horizontalSpacer_7 = new QSpacerItem(12, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_5->addItem(horizontalSpacer_7, 0, 0, 1, 1);

        esci_prg = new QPushButton(groupBox_5);
        esci_prg->setObjectName(QString::fromUtf8("esci_prg"));
        sizePolicy1.setHeightForWidth(esci_prg->sizePolicy().hasHeightForWidth());
        esci_prg->setSizePolicy(sizePolicy1);
        esci_prg->setMinimumSize(QSize(151, 60));
        esci_prg->setMaximumSize(QSize(151, 60));
        esci_prg->setBaseSize(QSize(151, 60));
        QIcon icon7;
        icon7.addFile(QString::fromUtf8(":/images/exit.png"), QSize(), QIcon::Normal, QIcon::Off);
        esci_prg->setIcon(icon7);
        esci_prg->setIconSize(QSize(32, 32));

        gridLayout_5->addWidget(esci_prg, 0, 1, 1, 1);

        horizontalSpacer_8 = new QSpacerItem(13, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_5->addItem(horizontalSpacer_8, 0, 2, 1, 1);


        gridLayout_6->addWidget(groupBox_5, 2, 3, 1, 2);

        horizontalSpacer_10 = new QSpacerItem(267, 93, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_6->addItem(horizontalSpacer_10, 2, 5, 1, 2);

        horizontalSpacer_13 = new QSpacerItem(272, 35, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_6->addItem(horizontalSpacer_13, 3, 1, 1, 1);

        groupBox_6 = new QGroupBox(frame_op);
        groupBox_6->setObjectName(QString::fromUtf8("groupBox_6"));
        gridLayout_8 = new QGridLayout(groupBox_6);
        gridLayout_8->setObjectName(QString::fromUtf8("gridLayout_8"));
        gridLayout_7 = new QGridLayout();
        gridLayout_7->setObjectName(QString::fromUtf8("gridLayout_7"));

        gridLayout_8->addLayout(gridLayout_7, 0, 0, 1, 1);


        gridLayout_6->addWidget(groupBox_6, 3, 2, 1, 4);

        horizontalSpacer_14 = new QSpacerItem(255, 35, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_6->addItem(horizontalSpacer_14, 3, 6, 1, 1);

        verticalSpacer = new QSpacerItem(97, 85, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout_6->addItem(verticalSpacer, 4, 3, 1, 1);

        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_6->addItem(horizontalSpacer_3, 0, 0, 1, 1);

        horizontalSpacer_5 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_6->addItem(horizontalSpacer_5, 1, 0, 1, 1);

        horizontalSpacer_11 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_6->addItem(horizontalSpacer_11, 0, 7, 1, 1);

        horizontalSpacer_12 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_6->addItem(horizontalSpacer_12, 1, 7, 1, 1);

        groupBox->raise();
        groupBox_2->raise();
        groupBox_3->raise();
        groupBox_4->raise();
        groupBox_5->raise();
        groupBox_6->raise();

        retranslateUi(frame_op);

        QMetaObject::connectSlotsByName(frame_op);
    } // setupUi

    void retranslateUi(QWidget *frame_op)
    {
        frame_op->setWindowTitle(QApplication::translate("frame_op", "Form", 0, QApplication::UnicodeUTF8));
        groupBox->setTitle(QApplication::translate("frame_op", "Anagrafica", 0, QApplication::UnicodeUTF8));
        libri->setText(QApplication::translate("frame_op", "Anagrafica libri", 0, QApplication::UnicodeUTF8));
        categoria->setText(QApplication::translate("frame_op", "Categorie libri", 0, QApplication::UnicodeUTF8));
        clienti->setText(QApplication::translate("frame_op", "Anagrafica clienti", 0, QApplication::UnicodeUTF8));
        groupBox_3->setTitle(QApplication::translate("frame_op", "Impostazioni", 0, QApplication::UnicodeUTF8));
        backup_restore->setText(QApplication::translate("frame_op", "Backup \n"
"and \n"
"restore", 0, QApplication::UnicodeUTF8));
        preferenze->setText(QApplication::translate("frame_op", "Preferenze", 0, QApplication::UnicodeUTF8));
        wizard_avvio->setText(QApplication::translate("frame_op", "Wizard di avvio", 0, QApplication::UnicodeUTF8));
        groupBox_2->setTitle(QApplication::translate("frame_op", "Prestiti e Scadenze", 0, QApplication::UnicodeUTF8));
        cl2->setText(QApplication::translate("frame_op", "Prestito libri", 0, QApplication::UnicodeUTF8));
        scadenze->setText(QApplication::translate("frame_op", "Scadenze", 0, QApplication::UnicodeUTF8));
        groupBox_4->setTitle(QApplication::translate("frame_op", "Informazioni", 0, QApplication::UnicodeUTF8));
        about_luxury->setText(QApplication::translate("frame_op", "About", 0, QApplication::UnicodeUTF8));
        about_qt->setText(QApplication::translate("frame_op", "About QT", 0, QApplication::UnicodeUTF8));
        groupBox_5->setTitle(QApplication::translate("frame_op", "Esci dal programma", 0, QApplication::UnicodeUTF8));
        esci_prg->setText(QApplication::translate("frame_op", "Esci", 0, QApplication::UnicodeUTF8));
        groupBox_6->setTitle(QApplication::translate("frame_op", "Visuazza scadenza", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class frame_op: public Ui_frame_op {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FRAME_OP_H
