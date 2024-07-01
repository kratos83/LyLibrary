#ifndef VERIFY_PIVA_H
#define VERIFY_PIVA_H

#include <QDialog>

namespace Ui {
class verify_piva;
}

class verify_piva : public QDialog
{
    Q_OBJECT
    
public:
    explicit verify_piva(QWidget *parent = 0);
    ~verify_piva();
    
public slots:
    void verify();
    QString read_piva(QString code);
private:
    Ui::verify_piva *ui;
};

#endif // VERIFY_PIVA_H
