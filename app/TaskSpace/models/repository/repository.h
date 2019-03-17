#ifndef REPOSITORY_H
#define REPOSITORY_H

#include <QObject>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QtDebug>

#include "models/settings_manager/settings_manager.h"

class Router;

class Repository : public QObject
{
    Q_OBJECT
public:
    explicit Repository(QObject *parent = nullptr);
    ~Repository();

    QString dbPath() const;
    void setDbPath(const QString &dbPath);

    QStringList getAvaliableStatuses();

private:
    QScopedPointer<SettingsManager> m_settingsManager;

    QString m_dbPath;

    void loadSettings();

    bool initDb(QString path="");

    friend class Router;

signals:
    void dbPathChanged(QString path);

public slots:
};

#endif // REPOSITORY_H
