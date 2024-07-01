#ifndef BARCODE_H
#define BARCODE_H

#include <QMainWindow>
#include <QPainter>
#include <QPrinter>
#include "ean13.h"

namespace Ui {
    class barcode;
}

class barcode : public QMainWindow
{
    Q_OBJECT

public:
    explicit barcode(QWidget *parent = 0);
    ~barcode();

protected:
    void paintEvent(QPaintEvent *event);
    void resizeEvent(QResizeEvent *event) ;

private:
    Ui::barcode *ui;
    EAN13   *barcode1 ;
    QString barcodeStr ;
    QString error;

    void  draw(QPainter *painter) ;

public slots:
    void  updateSwtClicked(QString code) ;
    void  printSwtClicked() ;

};

#endif // BARCODE_H
