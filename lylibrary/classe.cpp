#include "classe.h"

classe::classe(QWidget *parent):QLabel(parent)
{
}

classe::~classe()
{
}

void classe:: mousePressEvent(QMouseEvent *event)
{
    event = event;
    emit clicked();
}
