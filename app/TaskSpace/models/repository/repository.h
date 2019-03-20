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
    void changeTaskStatus(size_t taskIndex, QString status);
    int getTaskCountByStatus(QString status);
    Task getTaskByIndex(size_t taskIndex) const;

private:
    QScopedPointer<SettingsManager> m_settingsManager;

    QString m_dbPath;
    QList< QSharedPointer<Task> > m_tasks;

    void loadSettings();
    void loadMockData();

    bool initDb(QString path="");

    friend class Router;

signals:
    void dbPathChanged(QString path);
    void tasksUpdated();

public slots:
};

#endif // REPOSITORY_H
