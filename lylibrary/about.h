#ifndef ABOUT_H
#define ABOUT_H

#include <QDialog>

namespace Ui {
    class about;
}

class about : public QDialog {
    Q_OBJECT
public:
    about(QWidget *parent = nullptr);
    ~about();
    QString versione;

private slots:
   void close_ab();

private:
    Ui::about *ui;

};

#endif //ABOUT_H
