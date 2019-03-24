#ifndef SQLITE_ADAPTER_H
#define SQLITE_ADAPTER_H

#include <QObject>
#include <QtSql>
#include <QDebug>

class MySQLiteAdapter : public QObject
{
    Q_OBJECT
public:
    explicit MySQLiteAdapter(QObject *parent = nullptr);
    ~MySQLiteAdapter();

    static QList<QSqlRecord> executeSQL(QString dbPath, QString request);
    static QList<QSqlRecord> executeSQL(QString dbPath, QSqlQuery request);

private:

signals:

public slots:
};

#endif // SQLITE_ADAPTER_H
