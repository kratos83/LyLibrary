#ifndef RESIZEDIALOG_H
#define RESIZEDIALOG_H

#include <QDialog>

#define AVERAGE 0
#define BILINEAR 1

QT_BEGIN_NAMESPACE
class QPushButton;
class QLabel;
class QSpinBox;
class QComboBox;
class QCheckBox;
QT_END_NAMESPACE

class ResizeDialog : public QDialog
{
    Q_OBJECT

public:
    ResizeDialog(QWidget *parent=0, QSize size = QSize());
    ~ResizeDialog();

    void setDefaultSize(QSize size);

signals:
    //emit when user clicks on resizeButton
    void resizeImage(QSize newSize, int filter);

private slots:
    void reset();
    void accept();
    void checkRatioW();
    void checkRatioH();
    void wChanged(int);
    void hChanged(int);

private:
    QLabel *labelWidth;
    QLabel *labelHeight;
    QSpinBox *spinWidth;
    QSpinBox *spinHeight;
    QComboBox *comboQuality;
    QCheckBox *checkKeepRatio;
    QPushButton *resetButton;
    QPushButton *cancelButton;
    QPushButton *resizeButton;

    int dWidth;
    int dHeight;
    double ratio;

    bool wModified;
    bool hModified;
};

#endif // RESIZEDIALOG_H
