#ifndef SQLITE_ADAPTER_H
#define SQLITE_ADAPTER_H

#include <QObject>
#include <QtSql>
#include <QDebug>

class SQLiteAdapter : public QObject
{
    Q_OBJECT
public:
    explicit SQLiteAdapter(QObject *parent = nullptr);
    ~SQLiteAdapter();

    QList<QSqlRecord> executeSQL(QString dbPath, QString request);

private:

signals:

public slots:
};

#endif // SQLITE_ADAPTER_H
