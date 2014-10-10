#include "impo_iva_toto.h"
#include "QDebug"

impo_iva_toto::impo_iva_toto(QObject *parent) :
    QObject(parent),cl(0),ts(0),cn(0)
{
}

double impo_iva_toto::s_iva(double psiva){
    cl = psiva;
    return cl;
}

double impo_iva_toto::c_iva(double pciva){
    ts = pciva;
    return ts;
}

double impo_iva_toto::iva_tot(double ivatot){

    cn = ivatot;
    return cn;
}
