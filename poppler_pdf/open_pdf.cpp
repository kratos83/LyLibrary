#include "open_pdf.h"
#include <QPrinter>
#include <QPainter>
#include <QFile>

open_pdf::open_pdf(QWidget *parent) :
    QLabel(parent)
{

    currentPage = -1;
    doc = 0;
    rubberBand = 0;
    scaleFactor = 1.0;
    setAlignment(Qt::AlignCenter);
}

open_pdf::~open_pdf()
{
    delete doc;
}

Poppler::Document *open_pdf::document()
{
    return doc;
}

QMatrix open_pdf::matrix() const{

    return QMatrix(scaleFactor * physicalDpiX() / 72.0, 0,
                   0, scaleFactor * physicalDpiY() / 72.0,
                   0, 0);
}


void open_pdf::showPage(int page)
{
    if (page != -1 && page != currentPage + 1) {
        currentPage = page - 1;
        emit pageChanged(page);
    }

    QImage image = doc->page(currentPage)
                      ->renderToImage(scaleFactor * physicalDpiX(), scaleFactor * physicalDpiY());

    if (!searchLocation.isEmpty()) {
        QRect highlightRect = matrix().mapRect(searchLocation).toRect();
        highlightRect.adjust(-2, -2, 2, 2);
        QImage highlight = image.copy(highlightRect);
        QPainter painter;
        painter.begin(&image);
        painter.fillRect(image.rect(), QColor(0, 0, 0, 32));
        painter.drawImage(highlightRect, highlight);
        painter.end();
    }

    setPixmap(QPixmap::fromImage(image));
}

bool open_pdf::setDocument(const QString &filePath){

    QFile f;
    f.setFileName(filePath);
    f.open(QIODevice::ReadOnly);
    QByteArray pdf=f.readAll();
    Poppler::Document *oldDocument = doc;

    doc = Poppler::Document::loadFromData(pdf);
    if (doc) {

        delete oldDocument;
        doc->setRenderHint(Poppler::Document::Antialiasing);
        doc->setRenderHint(Poppler::Document::TextAntialiasing);
        searchLocation = QRectF();
        currentPage = -1;
        setPage(1);
    }
    return doc != 0;
}

void open_pdf::setPage(int page)
{
    if (page != currentPage + 1) {
        searchLocation = QRectF();
        showPage(page);
    }
}

void open_pdf::previous_page(int page)
{
   showPage(page);
}

void open_pdf::next_page(int page)
{
    showPage(page);
}

qreal open_pdf::scale() const
{
    return scaleFactor;
}

void open_pdf::setScale(qreal scale)
{
    if (scaleFactor != scale) {
           scaleFactor = scale;
           showPage();
       }
}
