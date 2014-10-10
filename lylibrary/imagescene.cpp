#include <QGraphicsSimpleTextItem>
#include <iostream>

#include "imagescene.h"

// We extend the QGraphicsScene class in order to get the
//mouse move events (pixel coordinate label in the status bar)
ImageScene::ImageScene(QObject *parent)
    :QGraphicsScene(parent)
{
}

void ImageScene::mouseMoveEvent(QGraphicsSceneMouseEvent *e)
{
    QGraphicsScene::mouseMoveEvent(e);
    emit mouseMoved(e);
}

void ImageScene::mousePressEvent(QGraphicsSceneMouseEvent *e)
{
    QGraphicsScene::mousePressEvent(e);
    emit mousePressed(e);
}
