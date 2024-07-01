#include "barcode.h"
#include "ui_barcode.h"
#include <QDebug>
#include <QSettings>

barcode::barcode(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::barcode)
{
    ui->setupUi(this);
    setGeometry(70,70, 70, 70) ;
    barcode1 = new EAN13(this);
}

/********************************************************************************/
void barcode::resizeEvent(QResizeEvent * /* event */)
{
}

/********************************************************************************/
void barcode::paintEvent(QPaintEvent *event)
{
event = event;		// evade compiler warning
QPainter painter(this);
painter.setRenderHint(QPainter::Antialiasing, true);

draw(&painter);
}

/********************************************************************************/
void barcode::draw(QPainter *painter)
{
barcode1->makePattern(barcodeStr);

qreal scale = painter->device()->physicalDpiX() / 25.4;	// dots per mm
barcode1->draw(QRectF(14, 50, 37.29 * scale, 25.93 * scale), painter);
}

/********************************************************************************/
void barcode::updateSwtClicked(QString code)
{

barcodeStr = code;
update() ;
}

/********************************************************************************/
void barcode::printSwtClicked()
{
QPrinter *printer = new QPrinter() ;
int Wd, Ht ;

if(width() > height())
 printer->setPageOrientation(QPageLayout::Landscape) ;
else
 printer->setPageOrientation(QPageLayout::Portrait) ;

Wd = printer->width() ;
Ht = printer->height() ;

QPainter *painter = new QPainter(this);
painter->begin(printer);

barcode1->makePattern(barcodeStr);

painter->drawText(50,30,tr("EAN13 Barcode test generator"));

painter->drawText(50,70,tr("Arbitrary size"));
barcode1->draw(QRectF(50, 80, Wd - 100, Ht - 300), painter) ;

painter->drawText(50, Ht - 160,"Nominal size (37.29mm x 25.93mm)");
qreal scale = painter->device()->logicalDpiX() / 25.4;	// dots per mm
barcode1->draw(QRectF(50, Ht - 150, 37.29 * scale, 25.93 * scale), painter);

painter->end();
}

/********************************************************************************/

barcode::~barcode()
{
    delete ui;
}

