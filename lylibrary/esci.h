#ifndef ESCI_H
#define ESCI_H

#include <QDialog>

namespace Ui {
    class esci;
}

class esci : public QDialog
{
    Q_OBJECT

public:
    explicit esci(QWidget *parent = 0);
    ~esci();

private:
    Ui::esci *ui;
};

#endif // ESCI_H
