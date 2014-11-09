#ifndef OPEN_PDF_H
#define OPEN_PDF_H

#include <QLabel>
#if QT_VERSION >= 0x050000
#include <poppler-qt5.h>
#else
#include <poppler-qt4.h>
#endif

class QRubberBand;

class open_pdf : public QLabel
{
    Q_OBJECT
public:
    explicit open_pdf(QWidget *parent = 0);
    ~open_pdf();
    Poppler::Document *document();
    QMatrix matrix() const;
    qreal scale() const;

signals:
    void pageChanged(int currentPage);

public slots:
    bool setDocument(const QString &filePath);
    void setPage(int page = -1);
    void setScale(qreal scale);
    void next_page(int page = -1);
    void previous_page(int page = -1);
private:
    void showPage(int page = -1);

    Poppler::Document *doc;
    int currentPage;
    QRubberBand *rubberBand;
    qreal scaleFactor;
    QRectF searchLocation;

};

#endif // OPEN_PDF_H
