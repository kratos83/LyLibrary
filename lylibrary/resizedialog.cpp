#include "resizedialog.h"

#include <QGridLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QSpinBox>
#include <QComboBox>
#include <QCheckBox>
#include <QSpacerItem>
#include <QSettings>

#include <iostream>
#include "pref.h"

ResizeDialog::ResizeDialog(QWidget *parent, QSize size)
    : QDialog(parent)
{
    setAttribute(Qt::WA_DeleteOnClose);
    setWindowTitle(tr("Ridimensiona immagine"));

    QVBoxLayout * mainLayout = new QVBoxLayout(this);
    QGridLayout * topLayout = new QGridLayout();
    QGridLayout * buttonLayout = new QGridLayout();

    labelWidth = new QLabel(tr("Width"), this);
    spinWidth = new QSpinBox(this);
    spinWidth->setMaximum(10000);

    labelHeight = new QLabel(tr("Height"), this);
    spinHeight = new QSpinBox(this);
    spinHeight->setMaximum(10000);

    comboQuality = new QComboBox(this);
    comboQuality->addItem(tr("Average")); //index 0 or AVERAGE
    comboQuality->addItem(tr("Bilinear")); //index 1 or BILINEAR

    checkKeepRatio = new QCheckBox(tr("Keep Aspect Ratio"), this);
    checkKeepRatio->setChecked(true);

    resetButton = new QPushButton(tr("Re&set"), this);
    cancelButton = new QPushButton(tr("&Cancella"), this);
    resizeButton = new QPushButton(tr("&Ridimensiona"), this);
    resizeButton->setDefault(true);

    topLayout->addWidget(labelWidth, 1, 0);
    topLayout->addWidget(spinWidth, 1, 1);
    topLayout->addWidget(new QLabel(tr("pixels"), this), 1, 2);

    topLayout->addWidget(labelHeight, 2, 0);
    topLayout->addWidget(spinHeight, 2, 1);
    topLayout->addWidget(new QLabel(tr("pixels"), this), 2, 2);

    QLabel *q = new QLabel(tr("Filter"), this);
    topLayout->addWidget(q, 3, 0);
    topLayout->addWidget(comboQuality, 3, 1);

    topLayout->addWidget(checkKeepRatio, 4, 0, 1, 3);

    buttonLayout->addWidget(resetButton, 0, 0);
    buttonLayout->addWidget(cancelButton, 0, 1);
    buttonLayout->addWidget(resizeButton, 0, 2);

    QSpacerItem *spacer = new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Expanding);

    mainLayout->addLayout(topLayout);
    mainLayout->addSpacerItem(spacer);
    mainLayout->addLayout(buttonLayout);

    setTabOrder(spinWidth, spinHeight);
    setTabOrder(spinHeight, checkKeepRatio);
    setTabOrder(checkKeepRatio, comboQuality);

    adjustSize();
    spinWidth->setFocus();

    connect(spinWidth, SIGNAL(valueChanged(int)), this, SLOT(wChanged(int)));
    connect(spinHeight, SIGNAL(valueChanged(int)), this, SLOT(hChanged(int)));
    connect(spinWidth, SIGNAL(editingFinished()), this, SLOT(checkRatioW()));
    connect(spinHeight, SIGNAL(editingFinished()), this, SLOT(checkRatioH()));
    connect(resetButton, SIGNAL(clicked()), this, SLOT(reset()));
    connect(cancelButton, SIGNAL(clicked()), this, SLOT(close()));
    connect(resizeButton, SIGNAL(clicked()), this, SLOT(accept()));

    setDefaultSize(size);
    setFixedSize(300,160);
    wModified = false;
    hModified = false;
}

ResizeDialog::~ResizeDialog()
{
}

void ResizeDialog::setDefaultSize(QSize size)
{
    dWidth = size.width();
    spinWidth->setValue(dWidth);
    dHeight = size.height();
    spinHeight->setValue(dHeight);

    ratio = (double)dWidth/(double)dHeight;
}

void ResizeDialog::reset()
{
    spinWidth->setValue(dWidth);
    spinHeight->setValue(dHeight);
}

void ResizeDialog::accept()
{
    if(spinWidth->value() == dWidth && spinHeight->value() == dHeight)
        close();    //Nothing changed
    else {
        emit resizeImage(QSize(spinWidth->value(), spinHeight->value()), comboQuality->currentIndex());
        QSettings set(".fabaria_gest/config.ini",QSettings::IniFormat);
        set.beginGroup("Image_resize");
        set.setValue("larghezza",spinWidth->value());
        set.setValue("altezza",spinHeight->value());
        set.endGroup();
        close();
    }
}

void ResizeDialog::checkRatioW()
{
    if (checkKeepRatio->isChecked() && wModified) {
        double res = (double)(spinWidth->value()) / ratio;
        spinHeight->setValue((int)(res+0.5));
        hModified = false;
        wModified = false;
    }
}

void ResizeDialog::checkRatioH()
{
    if (checkKeepRatio->isChecked() && hModified) {
        double res = (double)(spinHeight->value()) * ratio;
        spinWidth->setValue((int)(res+0.5));
        hModified = false;
        wModified = false;
    }
}

void ResizeDialog::wChanged(int)
{
    wModified = true;
}

void ResizeDialog::hChanged(int)
{
    hModified = true;
}
