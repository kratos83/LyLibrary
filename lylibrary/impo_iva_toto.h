#ifndef IMPO_IVA_TOTO_H
#define IMPO_IVA_TOTO_H

#include <QObject>

class impo_iva_toto : public QObject
{
    Q_OBJECT
public:
    explicit impo_iva_toto(QObject *parent = 0);
    double cl,cn,ts;

signals:
    
public slots:
    double s_iva(double psiva);
    double c_iva(double pciva);
    double iva_tot(double ivatot);
    
};

#endif // IMPO_IVA_TOTO_H
