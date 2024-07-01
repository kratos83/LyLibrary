#ifndef CHECK_DATABASE_H
#define CHECK_DATABASE_H

#include <QDialog>
#include <QtGui>

namespace Ui {
class check_database;
}

class check_database : public QDialog
{
    Q_OBJECT
    
public:
    explicit check_database(QWidget *parent = 0);
    ~check_database();
    
public slots:
    void analizza_database();
    void ripristina_database();
private:
    Ui::check_database *ui;
    QProcess *analizza;
    QProcess *ripristina;
    QString error;
    QSettings *general;
    QDir *settingsDir;
    QString getLineFromCommandOutput( QString command );
};

#endif // CHECK_DATABASE_H
