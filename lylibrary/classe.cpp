#include "classe.h"

classe::classe(QWidget *parent):QLabel(parent)
{
}

classe::classe(QString testo,QWidget *parent):QLabel(parent),m_testo(testo)
{
    setText(m_testo);
}

classe::~classe()
{
}

void classe:: mousePressEvent(QMouseEvent *event)
{
    event = event;
    emit clicked();
    if(!m_testo.isEmpty())
        emit clickMouse(m_testo);
}
