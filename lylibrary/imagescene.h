#ifndef IMAGESCENE_H
#define IMAGESCENE_H

#include <QObject>
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>


class QGraphicsSimpleTextItem;

class ImageScene : public QGraphicsScene
{
    Q_OBJECT
public:
    ImageScene(QObject *parent = 0);

signals:
    void mouseMoved(QGraphicsSceneMouseEvent *e);
    void mousePressed(QGraphicsSceneMouseEvent *e);

protected:
    void mouseMoveEvent(QGraphicsSceneMouseEvent *e);
    void mousePressEvent(QGraphicsSceneMouseEvent *e);
};

#endif // IMAGESCENE_H
