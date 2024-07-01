#include <QApplication>
#include <QMessageBox>
#include "db_bk_rs.h"

static void message()
{
    db_bk_rs *backup = new db_bk_rs();
    QMessageBox *box= new QMessageBox;
    box->setWindowTitle("Utility");
    box->setText(QObject::tr("Database"));
    box->setInformativeText(QString::fromUtf8(QObject::tr("Il database esiste già?Se esiste vai avanti.").replace("'","''").toStdString().c_str()));
    box->setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
    box->setDefaultButton(QMessageBox::Ok);
    int ret = box->exec();
    switch(ret){
    case QMessageBox::Ok:
         //Open database
         backup->exec();
         box->close();
         break;
    case QMessageBox::Cancel:
         //Close
         exit(0);
         break;
    }
}

int main(int argc, char **argv)
{
    QApplication app(argc, argv);
    message();

    return app.exec();
}
