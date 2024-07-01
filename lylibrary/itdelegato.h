#ifndef ITDELEGATO_H
#define ITDELEGATO_H

#include <QItemDelegate>


class coldxdelegato : public QItemDelegate
{

    Q_OBJECT

public:

    coldxdelegato (QObject *parent);

    void paint(QPainter *painter,
               const QStyleOptionViewItem &option,
               const QModelIndex &index) const;


};

class ColNumDelegato : public QItemDelegate
{

    Q_OBJECT

public:

    ColNumDelegato (QObject *parent);

    void paint(QPainter *painter,
               const QStyleOptionViewItem &option,
               const QModelIndex &index) const;

};

class ColDataDelegato : public QItemDelegate
{

    Q_OBJECT

public:

    ColDataDelegato (QObject *parent);

    void paint(QPainter *painter,
               const QStyleOptionViewItem &option,
               const QModelIndex &index) const;

};


#endif // ITDELEGATO_H
