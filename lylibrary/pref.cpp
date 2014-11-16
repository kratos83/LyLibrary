#include "pref.h"
#include <QListWidget>
#include <QListWidgetItem>
#include <QSettings>
#include "mainwindow.h"
#include <QtXml/QtXml>
#include <QtGui>
#include <QtSql>
#include <QSqlDatabase>
#include <QMessageBox>
#include <QGraphicsView>
#include <QSysInfo>
#include <QFileDialog>
#include <QGraphicsItem>
#include <QGraphicsScene>
#include "articoli.h"
#include "settingsmanager.h"
#include "resizedialog.h"

pref::pref(QWidget *parent) :
    QDialog(parent), Ui::pref()
{
    setupUi(this);
    setModal(true);
    connect(chiudi,SIGNAL(clicked()),this,SLOT(esci()));
    connect(menu_d,SIGNAL(pressed()),this,SLOT(menu_ex()));
    connect(apply,SIGNAL(clicked()),this,SLOT(applica()));
    manager = new QNetworkAccessManager(this);
    connect(manager,SIGNAL(finished(QNetworkReply*)),this,SLOT(self_update_parse(QNetworkReply*)));
    connect(en_update,SIGNAL(clicked(bool)),this,SLOT(agg_en()));
    connect(ds_update,SIGNAL(clicked(bool)),this,SLOT(agg_dis()));
    connect(update_b,SIGNAL(clicked()),this,SLOT(self_update()));
    connect(open_image,SIGNAL(clicked()),this,SLOT(apri()));
    connect(sel_tema,SIGNAL(currentIndexChanged(QString)),this,SLOT(seleziona_tema(const QString &)));
    connect(open_db,SIGNAL(clicked()),this,SLOT(connetti_database()));
    pwd->setEchoMode(QLineEdit::Password);
    settings = new SettingsManager(this);
    imageScene = new ImageScene(this);
    graphicsView = new QGraphicsView(imageScene, groupBox_7);
    graphicsView->setMouseTracking(true);
    gridLayout_11->addWidget(graphicsView, 0, 0, 1, 1);
    graphicsView->setStyleSheet("background-color: transparent");

    QStringList lista;
    lista << QObject::tr("Modern") << QObject::tr("Elegant") << QObject::tr("Nero");
    sel_tema->addItems(lista);
    menu_pref();
    interface();
    readsettings();
    visagg();
    combolanguage();
}


void pref::interface(){
    listWidget->setViewMode(QListView::IconMode);
    listWidget->setIconSize(QSize(84,84));
    listWidget->setMovement(QListView::Static);
    listWidget->setMaximumWidth(128);
    listWidget->setSpacing(12);
    listWidget->setCurrentRow(0);
}

void pref::readsettings(){

    menu_d->setChecked(settings->generalValue("MainWindow/menu",QVariant()).toBool());

    settings->generalValue("Version/version",QVariant()).toString();
    en_update->setChecked(settings->generalValue("Update/agg_en",QVariant()).toBool());
    ds_update->setChecked(settings->generalValue("Update/agg_dis",QVariant()).toBool());

    ip->setText(settings->generalValue("DatabaseServer/indirizzoip",QVariant()).toString());
    porta->setValue(settings->generalValue("DatabaseServer/portadb",QVariant()).toInt());
    db_rete->setText(settings->generalValue("DatabaseServer/nomedb",QVariant()).toString());
    utente->setText(settings->generalValue("DatabaseServer/userdb",QVariant()).toString());
    pwd->setText(settings->generalValue("DatabaseServer/passdb",QVariant()).toString());

    dir_image->setText(settings->generalValue("Image_resize/dir_image",QVariant()).toString());
    images = image_resize(settings->generalValue("Image_resize/dir_image",QVariant()).toString());

    alt_larg = new QLabel(this);
    alt_larg->clear();
    alt_larg->setText("Larghezza logo "+QString::number(settings->generalValue("Image_resize/larghezza",QVariant()).toInt())+" px\nAltezza logo     "+QString::number(settings->generalValue("Image_resize/altezza",QVariant()).toInt())+" px");

    sel_tema->setCurrentText(settings->generalValue("Tema/sel_tema",QVariant()).toString());

    //FONT APPLICATION
    QFont fnt;
    fnt.fromString(settings->generalValue("Application/font","Verdana,-1,11,5,50,0,0,0,0,0").toString());
    cmbFontName->setFont(fnt);
    spinFontSize->setValue(fnt.pixelSize());

    QFont appfnt;
    appfnt.fromString(settings->generalValue("Application/font","Verdana,-1,11,5,50,0,0,0,0,0").toString());
    cmbApplicationFontName->setFont(appfnt);
    spinApplicationFontSize->setValue(appfnt.pixelSize());


    QPixmap px = images.getPixmap();
    imagePixmapItem = imageScene->addPixmap(px);
    gridLayout_8->addWidget(alt_larg,1,0,1,1);
    alt_larg->show();

    combo_language->setCurrentText(settings->generalValue("Language/language",QVariant()).toString());
}

void pref::menu_ex(){
    if(menu_d->isChecked()){
       form->disable_menu();
    }
    else{
        form->enable_menu();
    }
}

void pref::agg_en(){
    if(en_update->isChecked() == true){
        update_b->setVisible(true);
    }
    else
        agg_dis();
}

void pref::agg_dis(){
    if(ds_update->isChecked() == true){
        update_b->setVisible(false);
    }
    else
        agg_en();
}

void pref::visagg(){

    if(settings->generalValue("Update/agg_en",QVariant()).toBool() == true){
        update_b->setVisible(true);
    }
    else{
        update_b->setVisible(false);
    }

    if(settings->generalValue("Update/agg_dis",QVariant()).toBool() == true){
        update_b->setVisible(false);
    }
    else{
        update_b->setVisible(true);
    }

}

void pref::applica(){

    settings->setGeneralValue("MainWindow/menu",menu_d->isChecked());
    //Salvataggio
    //FONT
    QFont selFont=cmbFontName->currentFont();
    selFont.setPixelSize(spinFontSize->value());
    settings->setGeneralValue("Application/font",selFont.toString());

    //FONT APPLICATION
    QFont appfnt=cmbApplicationFontName->currentFont();
    appfnt.setPixelSize(spinApplicationFontSize->value());
    settings->setGeneralValue("Application/applicationFont",appfnt.toString());

    settings->setGeneralValue("Update/agg_en",en_update->isChecked());
    settings->setGeneralValue("Update/agg_dis",ds_update->isChecked());

    settings->setGeneralValue("AGGIOR/auto_en",en_update->isChecked());
    settings->setGeneralValue("AGGIOR/auto_ds",ds_update->isChecked());

    settings->setGeneralValue("Tema/sel_tema",sel_tema->currentText());

    settings->setGeneralValue("Language/language",combo_language->currentText());

    readsettings();
    emit load_plugins();
}

void pref::menu_pref(){

        configButton = new QListWidgetItem(listWidget);
        configButton->setIcon(QIcon(":/images/preferences-system.png"));
        configButton->setText(tr("Impostazioni\ngenerali"));
        configButton->setTextAlignment(Qt::AlignHCenter);
        configButton->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

        databaseButton = new QListWidgetItem(listWidget);
        databaseButton->setIcon(QIcon(":/images/svn-commit.png"));
        databaseButton->setText(tr("Database"));
        databaseButton->setTextAlignment(Qt::AlignHCenter);
        databaseButton->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

        updateButton = new QListWidgetItem(listWidget);
        updateButton->setIcon(QIcon(":/images/system-software-update.png"));
        updateButton->setText(tr("Aggiornamento"));
        updateButton->setTextAlignment(Qt::AlignHCenter);
        updateButton->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

        fatturaButton = new QListWidgetItem(listWidget);
        fatturaButton->setIcon(QIcon(":/images/logo.png"));
        fatturaButton->setText(tr("Impostazioni\nfattura"));
        fatturaButton->setTextAlignment(Qt::AlignHCenter);
        fatturaButton->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

        connect(listWidget,SIGNAL(currentItemChanged(QListWidgetItem*,QListWidgetItem*)),this, SLOT(changePage(QListWidgetItem*,QListWidgetItem*)));
}

void pref::changePage(QListWidgetItem *corrente,QListWidgetItem *indietro){
    if(!corrente)
        corrente = indietro;
    stackedWidget->setCurrentIndex(listWidget->row(corrente));
}

void pref::apri(){
    QString fileName = QFileDialog::getOpenFileName(this,
                                                        tr("Open File"), QString(),
                                        tr("Images (*.png *.xpm *.jpg *.bmp);;Tutti i file(*.*);;JPG(*.jpg);;PNG(*.png);;BMP(*.bmp);;XPM(*.xpm)"));
        if (!fileName.isEmpty()) {
            QImage image(fileName);
            if (image.isNull()) {
                QMessageBox::information(this, tr("LyLibrary"),
                                         tr("Impossibile aprire %1.").arg(fileName));
                return;
            }
            QApplication::setOverrideCursor(Qt::WaitCursor);
            imageScene->clear();
            images = image_resize(fileName);
            QPixmap pix = images.getPixmap().mask();
            imagePixmapItem = imageScene->addPixmap(pix);
            imageScene->setSceneRect(imagePixmapItem->boundingRect());
            dir_image->setText(fileName);
            QApplication::restoreOverrideCursor();
            label_file();

            settings->setGeneralValue("Image_resize/dir_image",fileName);
            settings->setGeneralValue("Image_resize/larghezza",images.getPixmap().width());
            settings->setGeneralValue("Image_resize/altezza",images.getPixmap().height());
        }
        imageScene->clear();
        readsettings();
}

void pref::save_file(){

    QString fileName = QFileDialog::getSaveFileName(this,
                                tr("Salva le immagini"),
                                QString(),
                                tr("Images (*.png);;Tutti i file(*.*);;PNG(*.png)"));
            if (fileName.length() != 0) {
                    // Aggiunge estensione alla fine del file se non c'è
                    if (fileName.indexOf(".png") < 0) {
                        fileName += ".png";
                    }
                }

            if (fileName.isEmpty())
                return;

            QImageWriter imageWriter(fileName);
            if (!imageWriter.canWrite()) {
                    QMessageBox::warning(this, tr("LyLibrary"),
                                             tr("Impossibile scrivere %1: %2")
                                             .arg(fileName)
                                             .arg(imageWriter.errorString()));
                }

                QApplication::setOverrideCursor(Qt::WaitCursor);
                imageWriter.write(imagePixmapItem->pixmap().toImage());
                images = image_resize(fileName);
                graphicsView->resetMatrix();
                QApplication::restoreOverrideCursor();
                updateDisplay();
                settings->setGeneralValue("Image_resize/dir_image",fileName);
                settings->setGeneralValue("Image_resize/larghezza",images.getPixmap().width());
                settings->setGeneralValue("Image_resize/altezza",images.getPixmap().height());
                imageScene->clear();
                readsettings();
}

void pref::label_file(){

        classe *cl = new classe(groupBox_5);
        cl->setText(tr("Ridimensiona immagine"));
        cl->setCursor(QCursor(Qt::PointingHandCursor));
        cl->setAlignment(Qt::AlignCenter);
        cl->show();
        gridLayout_8->addWidget(cl, 0, 0, 1, 1);
        int altezza = images.getPixmap().height();
        int larghezza = images.getPixmap().width();
        alt_larg->setText(tr("Larghezza logo ")+QString::number(larghezza)+tr(" px\nAltezza logo     ")+QString::number(altezza)+" px");
        alt_larg->show();
        gridLayout_8->addWidget(alt_larg,1,0,1,1);
        QPushButton *save = new QPushButton();
        save->setText(tr("Salva immagine"));
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/images/document-save.png"));
        save->setIcon(icon1);
        gridLayout_8->addWidget(save,1,2,1,1);
        QSpacerItem *spacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
        gridLayout_8->addItem(spacer,1,1,1,1);
        connect(save,SIGNAL(clicked()),this,SLOT(save_file()));
        connect(cl,SIGNAL(clicked()),this,SLOT(showResizeDialog()));
}

void pref::resizeSwitch(QSize size, int filter){

    QApplication::setOverrideCursor(Qt::WaitCursor);
       QString fType;
       switch(filter)
       {
       case AVERAGE:
           images = images.resizeAverage(size.width(), size.height());
           images.getPixmap();
           fType = tr("Media");
           break;
       case BILINEAR:
           images = images.resizeBilinear(size.width(), size.height());
           images.getPixmap();
           fType = tr("Bilineare");
           break;
       }

       QApplication::restoreOverrideCursor();

       updateDisplay();
}

void pref::showResizeDialog(){

    ResizeDialog *diag = new ResizeDialog(this, imagePixmapItem->pixmap().size());
        diag->setModal(true);

        connect(diag, SIGNAL(resizeImage(QSize,int)), this, SLOT(resizeSwitch(QSize, int)));

        diag->exec();
}

void pref::updateDisplay(){

    QPixmap img(images.getPixmap());
    imagePixmapItem->setPixmap(img);
}

QGraphicsItem *pref::getPixmapItem(){

    return imagePixmapItem;
}

void pref::self_update(){

#if defined(Q_OS_LINUX) && defined(__x86_64__)
    QUrl url("http://www.codelinsoft.it/package/lylibrary/lylibrary-lin64.xml");
    manager->get(QNetworkRequest(QUrl(url)));
    connect(manager,SIGNAL(finished(QNetworkReply*)),this,SLOT(self_update_parse(QNetworkReply*)));
#elif defined(Q_OS_LINUX) && defined(__x86)
    QUrl url("http://www.codelinsoft.it/package/lylibrary/lylibrary-lin32.xml");
    manager->get(QNetworkRequest(QUrl(url)));
    connect(manager,SIGNAL(finished(QNetworkReply*)),this,SLOT(self_update_parse(QNetworkReply*)));
#elif defined(_WIN64)
    QUrl url("http://www.codelinsoft.it/package/lylibrary/lylibrary-win64.xml");
    manager->get(QNetworkRequest(QUrl(url)));
    connect(manager,SIGNAL(finished(QNetworkReply*)),this,SLOT(self_update_parse(QNetworkReply*)));
#elif defined(_WIN32)
    QUrl url("http://www.codelinsoft.it/package/lylibrary/lylibrary-win32.xml");
    manager->get(QNetworkRequest(QUrl(url)));
    connect(manager,SIGNAL(finished(QNetworkReply*)),this,SLOT(self_update_parse(QNetworkReply*)));
#elif defined(Q_OS_MACX)
    QUrl url("http://www.codelinsoft.it/package/lylibrary/lylibrary-macx.xml");
    manager->get(QNetworkRequest(QUrl(url)));
    connect(manager,SIGNAL(finished(QNetworkReply*)),this,SLOT(self_update_parse(QNetworkReply*)));
#endif
}

void pref::self_update_parse(QNetworkReply* reply){

            // Check file type
            QDomDocument doc( "XML" );
            if (!doc.setContent(reply->readAll(), true)) {
            return;
            }

            QDomElement root = doc.documentElement();
            if( root.tagName() != "appname" ){
                    return;
            }

            // Loop over main nodes
            latest_version.clear();
            filelist.clear();
            QDomNode mainnode = root.firstChild();
            while( !mainnode.isNull() ){
                    QDomNode subnode = mainnode.firstChild();
            if( !subnode.isNull() ) {
                // Loop over each elements in subnodes
                while(! subnode.isNull()){
                    QDomElement e = subnode.toElement();
                    if(!e.isNull()){
                        // Start parsing, finally
                                            if( e.tagName() == "version" ){
                                                    latest_version = e.attribute("value","");

                                            }
                                            else if( e.tagName() == "file" ){
                                                    filelist.push_back(e.attribute("value",""));
                                            }
                                    }
                                    subnode = subnode.nextSibling();
                            }
                    }
                    mainnode = mainnode.nextSibling();
            }

            // Compare current version with the one on the server
            if( settings->generalValue("Version/version",QVariant()).toString() < latest_version){
                    // Add the list of files to download list
                QString txtt= "http://www.codelinsoft.it/package/";

                QMessageBox *box= new QMessageBox(this);
                box->setWindowTitle(tr("LyLibrary"));
                box->setText(tr("Aggiornamento"));
                box->setInformativeText(tr("E' disponibile la nuova versione ")+versione+ tr(",se vuoi aggiornare clicca per aggiornare.\n"
                                        "Se clicchi ok si chiude il programma e si aggiorna il software alla nuova versione"));
                box->setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
                box->setDefaultButton(QMessageBox::Ok);
                int ret = box->exec();
                switch(ret){
                      case QMessageBox::Ok:
                       //Open update
                       up_dw(QString::fromLatin1(filelist.data()->toStdString().c_str()),txtt);
                       box->close();
                       break;
                      case QMessageBox::Cancel:
                       //Close
                       box->close();
                       break;
                       }

            }
            else if(reply->error()){
                QMessageBox::warning(this,tr("LyLibrary"),QString(tr("Errore...\n")).arg(reply->errorString()));
            }
            else{
                    // Update success dialog
                QSystemTrayIcon::MessageIcon is1 = QSystemTrayIcon::Information;
                form->trayIcon->showMessage(tr(""),tr("Stai usando la nuova versione"),is1,15*10000);
            }
}


void pref::up_dw(QString package, QString url){

    process = new QProcess(this);
#if defined(Q_OS_LINUX)
    process->start("./update -u "+url+" -p "+package);
#elif defined(Q_OS_WIN)
    process->start("C:\\LyLibrary\\update -u "+url+" -p "+package);
#elif defined(Q_OS_MACX)
    process->start("./update.app/Contents/MacOS/update -u"+url+" -p"+package);
#endif
}

void pref::seleziona_tema(const QString &sheetName)
{
    QFile file(":/qss/" + sheetName.toLower() + ".qss");
    file.open(QFile::ReadOnly);
    QString styleSheet = QLatin1String(file.readAll());

    qApp->setStyleSheet(styleSheet);
    qApp->setStyle(styleSheet);
}

bool pref::dir_server(){
    QString sgf = tr("Devi riavviare per rendere effettive le modifiche....");
    txt1->setText(sgf);
    return sgf.toStdString().c_str();
}

void pref::connetti_database()
{
    if(ip->text().length() == 0 || db_rete->text().length() == 0 || utente->text().length() == 0 || pwd->text().length() == 0){
        QMessageBox MsgBox2;
        MsgBox2.setText(QString::fromUtf8(tr("Errore di connessione al DB")));
        MsgBox2.setInformativeText(QString::fromUtf8(tr("Verificare che i dati siano corretti")));
        MsgBox2.setWindowTitle(tr("LyLibrary"));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/images/logo1.png"), QSize(), QIcon::Normal, QIcon::Off);
        MsgBox2.setWindowIcon(icon);
        MsgBox2.setIcon(QMessageBox::Warning);
        MsgBox2.exec();
        ip->setStyleSheet("background-color: rgb(249, 22, 5)");
        db_rete->setStyleSheet("background-color: rgb(249, 22, 5)");
        utente->setStyleSheet("background-color: rgb(249, 22, 5)");
        pwd->setStyleSheet("background-color: rgb(249, 22, 5)");
    }
    else{
    db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName(ip->text());
    db.setPort(porta->value());
    db.setDatabaseName(db_rete->text());
    db.setUserName(utente->text());
    db.setPassword(pwd->text());

    settingsManager->setGeneralValue("DatabaseServer/indirizzoip",ip->text());
    settingsManager->setGeneralValue("DatabaseServer/portadb",porta->value());
    settingsManager->setGeneralValue("DatabaseServer/nomedb",db_rete->text());
    settingsManager->setGeneralValue("DatabaseServer/userdb",utente->text());
    settingsManager->setGeneralValue("DatabaseServer/passdb",pwd->text());
    settingsManager->setGeneralValue("Database/databaselocale","server");

    if (db.open())
    {
        QSqlQuery query1;
        query1.exec("GRANT ALL ON lylibrary.* TO 'lylibrary'@'"+ip->text()+"' IDENTIFIED BY 'lylibrary'");
        messaggio();
    }
    else{
        QMessageBox MsgBox2;
        MsgBox2.setText(QString::fromUtf8(tr("Errore di connessione al DB")));
        MsgBox2.setInformativeText(QString::fromUtf8(tr("Impossibile connettersi al db.Controllare le impostazioni.")));
        MsgBox2.setWindowTitle(tr("LyLibrary"));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/images/logo1.png"), QSize(), QIcon::Normal, QIcon::Off);
        MsgBox2.setWindowIcon(icon);
        MsgBox2.setIcon(QMessageBox::Warning);
        MsgBox2.exec();
    }
    }
}

void pref::messaggio()
{
    QMessageBox *box= new QMessageBox(this);
    box->setWindowTitle(tr("Lylibrary"));
    box->setText(tr("Database"));
    box->setInformativeText(tr("Creazione db strutturata con successo...."));
    box->setStandardButtons(QMessageBox::Ok);
    box->setDefaultButton(QMessageBox::Ok);
    int ret = box->exec();
    switch(ret){
    case QMessageBox::Ok:
         //Close
        #ifdef Q_OS_LINUX
        QProcess *proc = new QProcess();
        proc->start("/opt/lylibrary/lylibrary");
        #endif
        #ifdef Q_OS_WIN
        QProcess *proc = new QProcess();
        proc->start("C:\\Lylibrary\\lylibrary.exe");
        #endif
        #ifdef Q_OS_MAC
        QProcess *proc = new QProcess();
        proc->start("/Applications/lylibrary.app");
        #endif
         box->close();
         exit(0);
         break;
    }
}

void pref::esci()
{
    close();
    form->impo->setEnabled(true);
}

void pref::combolanguage()
{
    const QDir lang_dir(":/language/lylibrary/");
        const QStringList files = lang_dir.entryList(QStringList() << "*.qm");
        foreach (QString lang_file, files) {
            lang_file = lang_file.split('.').first();
            combo_language->addItem(lang_file);
        }

        default_language = set_language();
        if(files.contains(default_language+".qm"))
            default_language = "English";
}

QString pref::set_language()
{
    return QLocale::languageToString(QLocale().language());
}

void pref::changeEvent(QEvent *e)
{
    QDialog::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        retranslateUi(this);
        break;
    default:
        break;
    }
}
