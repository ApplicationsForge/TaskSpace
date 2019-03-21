#ifndef REPOSITORY_H
#define REPOSITORY_H

#include <QObject>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QtDebug>

#include "models/settings_manager/settings_manager.h"
#include "models/types/task/task.h"

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

    QList<Task> getTasks() const;

    QList<Task> getTasks(QString status) const;

    void setTasks(const QList< QSharedPointer<Task> > &tasks);

    void addTask(Task task);

    void removeTask(size_t index);

    int getTaskCountByStatus(QString status);

    Task getTaskByIndex(size_t index) const;

    void updateTaskStatus(size_t index, QString status);

    void updateTaskInfo(size_t index,
                    QString title,
                    QString description,
                    QDate dueToDate,
                    bool dueToDateEnabled,
                    QTime estimatedTime,
                    QTime actualTime);

private:
    QScopedPointer<SettingsManager> m_settingsManager;

    QString m_dbPath;
    QList< QSharedPointer<Task> > m_tasks;

    void loadSettings();
    void loadMockData();

    bool initDb(QString path="");

    QSharedPointer<Task> findTask(size_t index) const;

    friend class Router;

signals:
    void dbPathChanged(QString path);
    void tasksUpdated();

public slots:
};

#endif // REPOSITORY_H
