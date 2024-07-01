#ifndef CLASSE__H
#define CLASSE__H

#include <QMouseEvent>
#include <QLabel>

class classe :public QLabel
{
        Q_OBJECT
public:
        classe(QWidget *parent = 0);
        classe(QString testo, QWidget *parent = nullptr);
        virtual ~classe();
protected:
        virtual void  mousePressEvent(QMouseEvent *event);
signals:
        void clicked();
        void clickMouse(QString m_testo);

private:
        QString m_testo;
};

#endif //CLASSE__H
