#ifndef read_pdf_H
#define read_pdf_H

#include <QMainWindow>
#include <QtGui>
#include "open_pdf.h"

namespace Ui {
class read_pdf;
}

class read_pdf : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit read_pdf(QWidget *parent = 0);
    ~read_pdf();
    
private:
    Ui::read_pdf *ui;
    open_pdf *documentWidget;
    QString last_path;
    QVector<qreal> scaleFactors;
public slots:
    void openfile(QString path);
    void zoom_document(int index);
    void valuespin();
private slots:
    void on_indietro_clicked();
    void on_avanti_clicked();
};

#endif // read_pdf_H
