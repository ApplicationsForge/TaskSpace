#ifndef REPOSITORY_H
#define REPOSITORY_H

#include <QObject>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QtDebug>

#include "models/settings_manager/settings_manager.h"
#include "models/types/task/task.h"
#include "models/types/sqlite_adapter/sqlite_adapter.h"

class Router;

class Repository : public QObject
{
    Q_OBJECT
public:
    explicit Repository(QObject *parent = nullptr);
    ~Repository();

    QString helloString() const;
    void setHelloString(const QString &helloString);

    QString dbPath() const;
    void setDbPath(const QString &dbPath);

    QList<Task> tasks() const;
    QList<Task> tasksByStatus(size_t statusId);
    void setTasks(const QList<Task> &tasks);

    QList<Status> statuses() const;
    void setStatuses(const QList<Status> &statuses);

    QList<Teammate> teammates() const;
    void setTeammates(const QList<Teammate> &teammates);

private:
    QString m_helloString = "Here is your TaskSpace!";
    QScopedPointer<SettingsManager> m_settingsManager;

    QString m_dbPath;

    QList<Task> m_tasks;
    QList<Status> m_statuses;
    QList<Teammate> m_teammates;

    void loadSettings();
    void loadStatuses();
    void loadTeammates();
    void loadTasks();

    Teammate findTeammate(size_t id);
    Status findStatus(size_t id);
    Task findTask(size_t id);

    static bool resolveDb(QString path);
    static bool initDb(QString path="");

    friend class Router;

signals:
    void dbPathChanged(QString path);
    void tasksUpdated(size_t statusId, QList<Task> tasks);

public slots:
    void changeTaskStatus(size_t taskId, size_t statusId);
};

#endif // REPOSITORY_H
