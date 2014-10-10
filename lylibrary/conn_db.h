#ifndef CONN_DB_H
#define CONN_DB_H
#include <QtSql>

class Database {

public:
    QSqlDatabase connect();
    void closeDb();

};


#endif // CONN_DB_H
