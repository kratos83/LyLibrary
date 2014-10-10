#include "itdelegato.h"
#include <QDate>


/*--------------------------------------------------------------------------------------------
Colonna numerica allineata a destra senza decimali
--------------------------------------------------------------------------------------------*/

coldxdelegato::coldxdelegato(QObject *parent)
    :QItemDelegate(parent)
{

}

void coldxdelegato::paint(QPainter *painter,
                 const QStyleOptionViewItem & option,
                 const QModelIndex & index) const
{

    QString text = index.model()->data(index, Qt::DisplayRole).toString();
    QStyleOptionViewItem myOption = option;
    myOption.displayAlignment = Qt::AlignRight | Qt::AlignVCenter;

    drawDisplay(painter, myOption, myOption.rect, text);
    drawFocus(painter, myOption, myOption.rect);

}

/*--------------------------------------------------------------------------------------------
Colonna numerica allineata a destra con decimali
--------------------------------------------------------------------------------------------*/

ColNumDelegato::ColNumDelegato(QObject *parent)
    :QItemDelegate(parent)
{

}

void ColNumDelegato::paint(QPainter *painter,
                 const QStyleOptionViewItem & option,
                 const QModelIndex & index) const
{

    double numero = index.model()->data(index, Qt::DisplayRole).toDouble();

    QString text = QString("%L1").arg(numero,13,'f',2);

    QStyleOptionViewItem myOption = option;
    myOption.displayAlignment = Qt::AlignRight | Qt::AlignVCenter;

    drawDisplay(painter, myOption, myOption.rect, text);
    drawFocus(painter, myOption, myOption.rect);

}


/*--------------------------------------------------------------------------------------------
Colonna data formattata
--------------------------------------------------------------------------------------------*/

ColDataDelegato::ColDataDelegato(QObject *parent)
    :QItemDelegate(parent)
{

}

void ColDataDelegato::paint(QPainter *painter,
                 const QStyleOptionViewItem & option,
                 const QModelIndex & index) const
{

    QDate text = index.model()->data(index, Qt::DisplayRole).toDate();

    QStyleOptionViewItem myOption = option;
    myOption.displayAlignment = Qt::AlignRight | Qt::AlignVCenter;

    drawDisplay(painter, myOption, myOption.rect, text.toString("dd-MM-yyyy"));
    drawFocus(painter, myOption, myOption.rect);

}
