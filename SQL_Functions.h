#ifndef SQL_FUNCTIONS_H
#define SQL_FUNCTIONS_H

#include <QSqlDatabase>
#include <QString>
#include <QSqlQuery>
#include <QSqlRecord>

class SQLFunctions
{
private:
public:
    static int get_first_free_id(QSqlDatabase &db, QString table, QString column)
    {
        QString query = "SELECT MAX(" + column + ") FROM " + table + ";";
        QSqlQuery q = db.exec(query);
        q.next();
        int lastid = q.record().value(0).toInt();

        return ++lastid;
    }

    static bool entry_exists(QSqlDatabase &db, QString table, QString column, QString value)
    {
        QString query = "SELECT " + column + " FROM " + table + " WHERE " + column + " = " + value + ";";
        QSqlQuery q = db.exec(query);
        q.next();
        return q.size() > 0;
    }
};

#endif // SQL_FUNCTIONS_H
