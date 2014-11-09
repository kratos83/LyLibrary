#include <QtGui/QApplication>
#include <QStringList>
#include <QtGui>
#include "update.h"
#include <stdio.h>

static bool parseArguments(const QStringList &argomento)
{
    QStringListIterator arg_cmd(argomento);
    arg_cmd.next();
    while(arg_cmd.hasNext())
    {
        const QString &a_arg = arg_cmd.next().toLower();
        if(a_arg == "-p" || a_arg == "--package"){
            return true;
        }
        else if(a_arg == "-u" || a_arg == "--url"){
            return true;
        }
        else if(a_arg == "-v" || a_arg == "--version"){
            qDebug() << QString::fromUtf8("La versione del software installato è: ") << "2.0.2";
            return false;
        }
        else if(a_arg == "-h" || a_arg == "--help")
        {
            qDebug() << "USA LA LINEA DI COMANDO:\t "<< "VISUALIZZA\n"
                     << "-p or --package \t" << "Selezione del pacchetto da scaricare\n"
                     << "-u or --url \t\t" << "Selezione dell'indirizzo internet:\n"
                                            "\t\t\t ESEMPIO: http://\n\n\n"
                     << "VISUALIZZA LA VERSIONE INSTALLATA DEL SOFTWARE:\n\n"
                     << "-v or --version \t" << "Versione del software\n\n\n"
                     << "VISUALIZZA LE INFORMAZIONI DEL PROGRAMMA:\n\n"
                     << "-h or --help \t\t" << "Informazioni del software.";
            return false;
        }
        else{
            qDebug() << "Comando non trovato: " << a_arg;
            return false;
        }
    }
    return true;
}

int main(int argc, char **argv)
{
    QApplication app(argc, argv);

    if(!parseArguments(app.arguments()))
    {
        return -1;
    }

    const QString arg1 = argv[1];

    const QString arg3 = argv[3];

    if((arg1.toLower() == "-u" || arg1.toLower() == "--url") && (arg3.toLower() == "-p" || arg3.toLower() == "--package")){
        QWidget *wid= new QWidget;
        update *manager = new update(wid,argv[2],argv[4]);
        manager->show();
    }
    else
    {
        parseArguments(app.arguments() << "-h");
        exit(0);
    }

    app.exec();
}
