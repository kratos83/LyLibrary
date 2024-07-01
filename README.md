LyLibrary 3.1
=========

Software per la gestione delle bibioteche
------------------------------------------------------------------------
ITA LANG: LyLibrary fornisce tutto l'occorrente per la catalogazione dei libri, la gestione dei prestiti e l'archiviazione dei lettori della biblioteca. Il programma è rivolto non solo ai gestori di librerie o biblioteche ma anche agli appassionati di lettura che vogliono gestire i libri della propria collezione in maniera rapida ed efficace.

ENG LANG:  LyLibrary provides everything necessary to the books cataloguing, the loans management and the registration of the library readers. The program is not only addressed to the bookshops or libraries managers but also to all readings lovers that want to manage the books of their own collection in a fast and effective way.

---------------------------------------------------------------------------
Per poter installare LyLibrary da sorgente bisogna 
scaricare il programma all'indirizzo:

http://www.codelinsoft.it/sito/2013-11-17-17-56-34/lylibrary.html

Una volta scaricato aprire il terminale e dicitare i seguenti comandi

qt5-qtbase
qt5-qtbase-mysql
-------------------------------------------
Se utilizzato come server mysql installare
mysql-server
mysql-client

altrimenti utilizzare MariaDB

mariadb sia client che server
--------------------------------------------


---------------------------------------------
Installazione LyLibrary
tar -xvzf lylibrary-3.1.tar.gz

cd lylibrary-3.1
Compilare con le Qt in versione 5 e
eseguire il seguente comando

cmake -DMAKE_INSTALL_PREFIX=directory

make-j5
sudo make install

# LyLibrary
# LyLibrary
