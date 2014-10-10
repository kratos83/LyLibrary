#ifndef IMAGE_RESIZE_H
#define IMAGE_RESIZE_H

#include <QImage>
#include <iostream>
#include <QString>
#include <QPixmap>
#include <math.h>

class image_resize
{
public:
    image_resize();
    image_resize(QImage qImage);
    image_resize resizeAverage(int newwidth, int newheight);
    image_resize resizeBilinear(int newwidth, int newheight);
    QPixmap getPixmap();
    image_resize(QString);

private:
    QImage image;
};

#endif // IMAGE_RESIZE_H
