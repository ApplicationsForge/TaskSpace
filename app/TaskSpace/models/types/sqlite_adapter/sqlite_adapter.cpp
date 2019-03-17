#include "sqlite_adapter.h"

SQLiteAdapter::SQLiteAdapter(QObject *parent) : QObject(parent)
{
}

SQLiteAdapter::~SQLiteAdapter()
{

}

QList<QSqlRecord> SQLiteAdapter::executeSQL(QString dbPath, QString request)
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(dbPath);
    if(!db.open())
    {
       throw std::runtime_error("database is not connected");
    }

    qDebug() << "Request" << request;
    QList<QSqlRecord> response;

    QSqlQuery query;
    if(query.prepare(request))
    {
        if(query.exec())
        {
            if(query.lastError().text() != " ")
            {
                throw std::runtime_error(query.lastError().text().toStdString());
            }
            while (query.next())
            {
                //qDebug() << query.record();
                response.push_back(query.record());
                //QString parametr = query.value(0).toString();
                //qDebug() << parametr;
                //response.push_back(parametr);
            }
        }
        else
        {
            throw std::runtime_error("Невозможно выполнить запрос!");
        }
    }
    else
    {
        throw std::runtime_error("Невозможно подготовить запрос!");
    }

    /*QString qs;
    {
        qs.append(QSqlDatabase::database().connectionName());
    }
    QSqlDatabase::removeDatabase(qs);*/

    QSqlDatabase::removeDatabase(QSqlDatabase::database().connectionName());
    qDebug() << "AfterDelete" << QSqlDatabase::database().connectionNames();

    return response;
}
