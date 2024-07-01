#include "StampaBiblioteca.h"
#include <iostream>
#include <QtWidgets>

StampaBiblioteca::StampaBiblioteca(QObject *parent) : QObject(parent) 
{
    // Creazione oggetto printer (puntatore)
    // ed impostazioni varie sul formato pagina, nome file di appoggio
    // formato file prodotto
    // impostazione margine e risoluzione
     printer = new QPrinter();

     //Apro e chiudo database
     ;
     
    // Formato A4 = 210*297 millimetri
    // Formato A4 = 210*297 millimetri

    printer->setPageMargins(QMarginsF(5.0,5.0,5.0,5.0),QPageLayout::Millimeter);
    printer->setPageSize(QPageSize(QPageSize::A4));
    #if defined (Q_OS_UNIX) || defined (Q_OS_FREEBSD)
    printer->setOutputFormat(QPrinter::PdfFormat);
    #endif
    #ifdef Q_OS_MAC
    printer->setOutputFormat(QPrinter::PdfFormat);
    #endif
    printer->setResolution(254);
}

StampaBiblioteca::~StampaBiblioteca()
{
    delete printer;
}

void StampaBiblioteca::stampaBiblioteca(QPrinter* printer, QSqlQueryModel* model, 
                                  QTextDocument* docu)
{
    printer->setResolution(100);
    printer->setPageMargins(QMarginsF(5.0,5.0,5.0,5.0),QPageLayout::Millimeter);
    printer->setPageSize(QPageSize(QPageSize::A4));
    printer->setPageOrientation(QPageLayout::Landscape);
    int pag = 1;
    int rec = 0;
    while(model->canFetchMore())
        model->fetchMore();

    while(rec < model->rowCount())
    {
        for (int secRow = 0; secRow < model->rowCount(); ++secRow)
        {
            QSqlRecord record = model->record(rec);
            if(!record.isEmpty())
            {
                htmlRow += QString("<tr valign=\"top\">");
                htmlRow += QString("<td width=\"100\" >");
                htmlRow += QString("<p align=\"left\"><font size=\"1.5\" style=\"font-size: 10pt\">"+record.value("cod_articolo").toString()+"</font></p>");
                htmlRow += QString("</td>");
                htmlRow += QString("<td width=\"100\" >");
                htmlRow += QString("<p align=\"left\"><font size=\"1.5\" style=\"font-size: 10pt\">"+record.value("codbarre").toString()+"</font></p>");
                htmlRow += QString("</td>");
                htmlRow += QString("<td width=\"100\" >");
                htmlRow += QString("<p align=\"left\"><font size=\"1.5\" style=\"font-size: 10pt\">"+record.value("nome_articolo").toString()+"</font></p>");
                htmlRow += QString("</td>");
                htmlRow += QString("<td width=\"40\" >");
                htmlRow += QString("<p align=\"left\"><font size=\"1.5\" style=\"font-size: 10pt\">"+record.value("descrizione").toString()+"</font></p>");
                htmlRow += QString("</td>");
                htmlRow += QString("<td width=\"100\" >");
                htmlRow += QString("<p align=\"left\"><font size=\"1.5\" style=\"font-size: 10pt\">"+record.value("autore").toString()+"</font></p>");
                htmlRow += QString("</td>");
                htmlRow += QString("<td width=\"100\" >");
                htmlRow += QString("<p align=\"left\"><font size=\"1.5\" style=\"font-size: 10pt\">"+record.value("lingua").toString()+"</font></p>");
                htmlRow += QString("</td>");
                htmlRow += QString("<td width=\"100\" >");
                htmlRow += QString("<p align=\"left\"><font size=\"1.5\" style=\"font-size: 10pt\">"+record.value("categ").toString()+"</font></p>");
                htmlRow += QString("</td>");
                htmlRow += QString("<td width=\"100\" >");
                htmlRow += QString("<p align=\"left\"><font size=\"1.5\" style=\"font-size: 10pt\">"+record.value("scaffale").toString()+"</font></p>");
                htmlRow += QString("</td>");
                htmlRow += QString("<td width=\"100\" >");
                htmlRow += QString("<p align=\"left\"><font size=\"1.5\" style=\"font-size: 10pt\">"+record.value("quantita").toString()+"</font></p>");
                htmlRow += QString("</td>");
                htmlRow += QString("<td width=\"100\" >");
                htmlRow += QString("<p align=\"left\"><font size=\"1.5\" style=\"font-size: 10pt\">"+record.value("infoeditore").toString()+"</font></p>");
                htmlRow += QString("</td>");
            rec++;
        }
        }
        stampaBibliotecaPag(pag, docu, printer);
        if(rec < model->rowCount()){
            printer->newPage();
            stampaBibliotecaPag(pag, docu, printer);
            ++pag;
        }
    }
}

void StampaBiblioteca::stampaBibliotecaPag(int pagina, QTextDocument *docu,  QPrinter *printer)
{
    html += QString("<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0 Transitional//EN\">"
    "<html>"
    "<head>"
        "<meta http-equiv=\"content-type\" content=\"text/html; charset=utf-8\"/>"
        "<style type=\"text/css\">"
            "@page { size: landscape; margin: 2cm }"
            "p { margin-bottom: 0.25cm; line-height: 120% }"
            "td p { margin-bottom: 0cm; line-height: 120% }"
            "a:link { so-language: zxx }"
        "</style>"
    "</head>"
    "<body lang=\"it-IT\" dir=\"ltr\">"
    "<td width=\"100\" >"
        "<p align=\"center\" style=\"margin-bottom: 0cm;\"><b>Elenco biblioteca</b></p>"
    "</td>"
    "<table width=\"100\" cellpadding=\"0\" cellspacing=\"0\" border=\"0.5\">"
        "<col width=\"100*\">"
        "<col width=\"100*\">"
        "<col width=\"100*\">"
        "<col width=\"100*\">"
        "<col width=\"100*\">"
        "<col width=\"100*\">"
        "<col width=\"100*\">"
        "<col width=\"100*\">"
        "<col width=\"100*\">"
        "<col width=\"100*\">"
        "<tr valign=\"top\">"
            "<td width=\"100\" >"
                "<p align=\"left\"><font size=\"1.5\" style=\"font-size: 10pt\"><b>ID</b></font></p>"
            "</td>"
            "<td width=\"100\" >"
                "<p align=\"left\"><font size=\"1.5\" style=\"font-size: 10pt\"><b>Codice a barre</b></font></p>"
            "</td>"
            "<td width=\"100\" >"
                "<p align=\"left\" ><font size=\"1.5\" style=\"font-size: 10pt\"><b>Titolo</b></font></p>"
            "</td>"
            "<td width=\"100\" >"
                "<p align=\"left\"><font size=\"1.5\" style=\"font-size: 10pt\"><b>Descrizione</b></font></p>"
            "</td>"
            "<td width=\"100\" >"
                "<p align=\"left\"><font size=\"1.5\" style=\"font-size: 10pt\"><b>Autore</b></font></p>"
            "</td>"
            "<td width=\"100\" >"
                "<p align=\"left\"><font size=\"1.5\" style=\"font-size: 10pt\"><b>Lingua</b></font></p>"
            "</td>"
            "<td width=\"100\" >"
                "<p align=\"left\"><font size=\"1.5\" style=\"font-size: 10pt\"><b>Categoria</b></font></p>"
            "</td>"
            "<td width=\"100\" >"
                "<p align=\"left\"><font size=\"1.5\" style=\"font-size: 10pt\"><b>Collocazione</b></font></p>"
            "</td>"
            "<td width=\"100\" >"
                "<p align=\"left\"><font size=\"1.5\" style=\"font-size: 10pt\"><b>Quantità</b></font></p>"
            "</td>"
            "<td width=\"100\" >"
                "<p align=\"left\"><font size=\"1.5\" style=\"font-size: 10pt\"><b>Info editore</b></font></p>"
            "</td>"
        "</tr>");
        html += htmlRow;
        html += QString("</tr></table>");
        html += QString("</body></html>");
        QSizeF paperSize;
        paperSize.setWidth(printer->width());
        paperSize.setHeight(printer->height());
        QFont myfont("Verdana", 10);
        docu->setDefaultFont(myfont);
        docu->setHtml(html);
        docu->setPageSize(paperSize);
        docu->print(printer);
}
