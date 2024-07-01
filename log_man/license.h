#ifndef LICENSE_H
#define LICENSE_H

#include <QDialog>
#include <QSettings>
#include <QtGui>
#include <QDir>

namespace Ui {
    class license;
}

class license : public QDialog
{
    Q_OBJECT

public:
    explicit license(QWidget *parent = 0);
    ~license();

public slots:
    void button();
    void esci();
    void page_stack();
    void page_fine();
    void indietro();
    void avanti();
    void read_conf();
    void traduzione();

private:
    Ui::license *ui;
    QDir *settingsDir;
    QSettings *general;
    QProcess *myprocess;

protected:
    void closeEvent(QCloseEvent *);
signals:
    void load_license();
    void outlogtext(QString ver);
};

#endif // LICENSE_H
