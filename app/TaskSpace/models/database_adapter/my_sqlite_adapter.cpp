#include "my_sqlite_adapter.h"

MySQLiteAdapter::MySQLiteAdapter(QObject *parent) : QObject(parent)
{
}

MySQLiteAdapter::~MySQLiteAdapter()
{

}

QList<QSqlRecord> MySQLiteAdapter::executeSQL(QString dbPath, QString request)
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(dbPath);
    if(!db.open())
    {
        qDebug() << "MySQLiteAdapter::executeSQL:" << "database is not connected";
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
                QSqlDatabase::removeDatabase(QSqlDatabase::database().connectionName());
                qDebug() << "MySQLiteAdapter::executeSQL:" << query.lastError().text();
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
            QSqlDatabase::removeDatabase(QSqlDatabase::database().connectionName());
            qDebug() << "MySQLiteAdapter::executeSQL:" << "Невозможно выполнить запрос!";
            throw std::runtime_error("Невозможно выполнить запрос!");
        }
    }
    else
    {
        QSqlDatabase::removeDatabase(QSqlDatabase::database().connectionName());
        qDebug() << "MySQLiteAdapter::executeSQL:" << "Невозможно подготовить запрос!";
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

QList<QSqlRecord> MySQLiteAdapter::executeSQL(QString dbPath, QSqlQuery request)
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(dbPath);
    if(!db.open())
    {
        qDebug() << "MySQLiteAdapter::executeSQL:" << "database is not connected";
        throw std::runtime_error("database is not connected");
    }

    QList<QSqlRecord> response;

    QSqlQuery query;
    if(request.isValid())
    {
        if(query.exec())
        {
            if(query.lastError().text() != " ")
            {
                QSqlDatabase::removeDatabase(QSqlDatabase::database().connectionName());
                qDebug() << "MySQLiteAdapter::executeSQL:" << query.lastError().text();
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
            QSqlDatabase::removeDatabase(QSqlDatabase::database().connectionName());
            qDebug() << "MySQLiteAdapter::executeSQL:" << "Невозможно выполнить запрос!";
            throw std::runtime_error("Невозможно выполнить запрос!");
        }
    }
    else
    {
        QSqlDatabase::removeDatabase(QSqlDatabase::database().connectionName());
        qDebug() << "MySQLiteAdapter::executeSQL:" << "request is not valid!";
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
