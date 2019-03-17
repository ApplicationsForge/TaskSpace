#ifndef REPOSITORY_H
#define REPOSITORY_H

#include <QObject>

#include "models/settings_manager/settings_manager.h"

class Router;

class Repository : public QObject
{
    Q_OBJECT
public:
    explicit Repository(QObject *parent = nullptr);

    QString helloString() const;
    void setHelloString(const QString &helloString);

    QString dbPath() const;
    void setDbPath(const QString &dbPath);

private:
    QString m_helloString = "Here is your TaskSpace!";
    QScopedPointer<SettingsManager> m_settingsManager;

    QString m_dbPath;

    void loadSettings();

    friend class Router;

signals:
    void dbPathChanged(QString path);

public slots:
};

#endif // REPOSITORY_H
