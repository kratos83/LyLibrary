#include "classe.h"

classe::classe(QWidget *parent):QLabel(parent)
{
}

classe::~classe()
{
}

void classe:: mousePressEvent(QMouseEvent *event)
{
        emit clicked();
}
