#ifndef LYLIBRARYMESSAGEBOX_H
#define LYLIBRARYMESSAGEBOX_H

#include <QtWidgets>

namespace Ui
{class LyLibraryMessageBox;}

class LyLibraryMessageBox : public QDialog
{
    Q_OBJECT
    
public:
    
    enum LyLibraryMessageBoxButtonRole{
        LyLibraryMessageBoxBoxAcceptRole,
        LyLibraryMessageBoxBoxRejectRole,
        LyLibraryMessageBoxBoxYesRole,
        LyLibraryMessageBoxBoxNoRole
    };
    
    enum CCode { Rejected, Accepted };
    
    enum CIcon{
        CInformationIcon,
        CWarningIcon,
        CFailedIcon,
        CNoIcon,
        CCriticalIcon,
        CExitIcon
    };

    
    LyLibraryMessageBox(QWidget* parent=0);
    ~LyLibraryMessageBox();

    //Aggiungo il bottone
    void addButton(QPushButton *button, LyLibraryMessageBoxButtonRole type);
    //Aggiungo icona
    void setIcon(CIcon icona);
    //Clicco il bottone
    QAbstractButton *clickedButton() const;
    //Titolo
    void setWindowTitleLyLibrary(QString title);
    //Testo da mettere prima del testo principale
    void setText(QString text);
    //Visualizzo il testo
    void setInformativeText(const QString &text);

    
    //Static command
    static LyLibraryMessageBoxButtonRole information(QWidget *parent, const QString &title,
         const QString &text);
    static LyLibraryMessageBoxButtonRole failed(QWidget *parent, const QString &title,
         const QString &text);
    static LyLibraryMessageBoxButtonRole warning(QWidget *parent, const QString &title,
         const QString &text);
    static LyLibraryMessageBoxButtonRole critical(QWidget *parent, const QString &title,
         const QString &text);
    static LyLibraryMessageBoxButtonRole esciLyLibrary(QWidget *parent, const QString &title,
                                                  const QString &text);
    static LyLibraryMessageBoxButtonRole showMessage(QWidget *parent, LyLibraryMessageBox::CIcon icon,
            const QString& title, const QString& text);

    QAbstractButton *bottone;
    QAbstractButton *escapeButton;
    QDialogButtonBox *buttonBox;
    QPushButton *defaultButton;
public slots:
    void buttonClicked(QAbstractButton* botton);
private:
   QString m_text, m_title;
    Ui::LyLibraryMessageBox *ui;
    bool compatMode;
    QList<QAbstractButton *> customButtonList;
    
signals:
    void buttonClickedMessage(QAbstractButton *button);
};
#endif // MESSAGEBOX_H
