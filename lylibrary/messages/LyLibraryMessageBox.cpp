#include "LyLibraryMessageBox.h"
#include "../ui_LyLibraryMessageBox.h"
#include <QtDebug>

LyLibraryMessageBox::LyLibraryMessageBox(QWidget* parent) :
    QDialog(parent),
    ui(new Ui::LyLibraryMessageBox),
    buttonBox(new QDialogButtonBox)
{
    ui->setupUi(this);
    QScreen *screen = QGuiApplication::primaryScreen();
    QRect  reale = screen->geometry();
    int x = (reale.width()-width())/2;
    int y = (reale.height()-height())/2;
    setGeometry(x,y,width(),height());
}

void LyLibraryMessageBox::addButton(QPushButton* button, LyLibraryMessageBox::LyLibraryMessageBoxButtonRole type)
{
    bottone = button;
    buttonBox->addButton(bottone,(QDialogButtonBox::ButtonRole)type);
    connect(buttonBox, SIGNAL(clicked(QAbstractButton*)),
        this, SLOT(buttonClicked(QAbstractButton*)));
    ui->gridButton->addWidget(buttonBox,ui->gridButton->rowCount(),0,1,ui->gridButton->columnCount());
}

void LyLibraryMessageBox::buttonClicked(QAbstractButton* botton)
{
    bottone = botton;
    done(LyLibraryMessageBox::Accepted);
    emit  buttonClickedMessage(botton);
}

void LyLibraryMessageBox::setIcon(LyLibraryMessageBox::CIcon icona)
{
    switch(icona)
    {
        case CInformationIcon:
        {
            ui->icona->setVisible(true);
            QIcon pix(":/images/info.png");
            ui->icona->setPixmap(pix.pixmap(128,128));
        }
        break;
        case CWarningIcon:
        {
            ui->icona->setVisible(true);
            QIcon pix(":/images/attenzione.png");
            ui->icona->setPixmap(pix.pixmap(128,128));
        }
        break;
        case CFailedIcon:
        {
            ui->icona->setVisible(true);
            QIcon pix(":/images/stop.png");
            ui->icona->setPixmap(pix.pixmap(128,128));
        }
        break;
        case CCriticalIcon:
        {
            ui->icona->setVisible(true);
            QIcon pix(":/images/alert.png");
            ui->icona->setPixmap(pix.pixmap(128,128));
        }
        break;
        case CExitIcon:
        {
            ui->icona->setVisible(true);
            QIcon pix(":/images/application-exit.png");
            ui->icona->setPixmap(pix.pixmap(128,128));
        }
        break;
        case CNoIcon:
        {
            ui->icona->setPixmap(QPixmap());
            ui->icona->setVisible(false);
        }
        break;
        default:
        {
            ui->icona->setPixmap(QPixmap());
            ui->icona->setVisible(false);
        }
    }
}

void LyLibraryMessageBox::setInformativeText(const QString& text)
{
    ui->text->setText(text);
}

void LyLibraryMessageBox::setText(QString text)
{
    ui->testoSecondo->setText(text);
}

void LyLibraryMessageBox::setWindowTitleLyLibrary(QString title)
{
    setWindowTitle(title);
}

QAbstractButton * LyLibraryMessageBox::clickedButton() const
{
    return bottone;
}

LyLibraryMessageBox::LyLibraryMessageBoxButtonRole LyLibraryMessageBox::critical(QWidget* parent, const QString& title, const QString& text)
{
    return showMessage(parent, LyLibraryMessageBox::CCriticalIcon, title, text);
}

LyLibraryMessageBox::LyLibraryMessageBoxButtonRole LyLibraryMessageBox::information(QWidget* parent, const QString& title, const QString& text)
{
    return showMessage(parent, LyLibraryMessageBox::CInformationIcon, title, text);
}

LyLibraryMessageBox::LyLibraryMessageBoxButtonRole LyLibraryMessageBox::failed(QWidget* parent, const QString& title, const QString& text)
{
    return showMessage(parent, LyLibraryMessageBox::CFailedIcon, title, text);
}

LyLibraryMessageBox::LyLibraryMessageBoxButtonRole LyLibraryMessageBox::warning(QWidget* parent, const QString& title, const QString& text)
{
    return showMessage(parent, LyLibraryMessageBox::CWarningIcon, title, text);
}

LyLibraryMessageBox::LyLibraryMessageBoxButtonRole LyLibraryMessageBox::esciLyLibrary(QWidget* parent, const QString& title, const QString& text)
{
    return showMessage(parent, LyLibraryMessageBox::CExitIcon, title, text);
}

LyLibraryMessageBox::LyLibraryMessageBoxButtonRole LyLibraryMessageBox::showMessage(QWidget* parent, LyLibraryMessageBox::CIcon icon,
                                                                                                                        const QString& title, const QString& text)
{    
    Q_UNUSED(parent)
    LyLibraryMessageBox msgBox;
    msgBox.setIcon(icon);
    msgBox.setWindowTitleLyLibrary(title);
    msgBox.setText(title);
    msgBox.setInformativeText(text);
    QPushButton *m_pushOk = new QPushButton;
    m_pushOk->setText("Ok");
    m_pushOk->setIcon(QIcon(":/images/online.png"));
    msgBox.addButton(m_pushOk,LyLibraryMessageBox::LyLibraryMessageBoxBoxAcceptRole);
    QPushButton *m_pushCancel= new QPushButton;
    m_pushCancel->setText("Cancella");
    m_pushCancel->setIcon(QIcon(":/images/stop.png"));
    msgBox.addButton(m_pushCancel,LyLibraryMessageBox::LyLibraryMessageBoxBoxRejectRole);
    msgBox.exec();
    if(msgBox.clickedButton() == m_pushOk){
        msgBox.close();
    }
    else if(msgBox.clickedButton() == m_pushCancel){
        msgBox.close();
    }
    
    return LyLibraryMessageBox::LyLibraryMessageBoxBoxAcceptRole;
}

LyLibraryMessageBox::~LyLibraryMessageBox()
{
    delete ui;
}
