#ifndef FIND_CAP_ITALIAN_H
#define FIND_CAP_ITALIAN_H

#include <QDialog>

namespace Ui {
class find_cap_italian;
}

class find_cap_italian : public QDialog
{
    Q_OBJECT
    
public:
    explicit find_cap_italian(QWidget *parent = 0);
    ~find_cap_italian();
    
private:
    Ui::find_cap_italian *ui;
    QString cap;
    QString found;
public slots:
    void find_comune();
    void vis_cap_comune();
    void cap_trovato();

};

#endif // FIND_CAP_ITALIAN_H
