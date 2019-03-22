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

    QStringList getAvaliableStatuses();

    QList<Task> getTasks() const;

    QList<Task> getTasks(QString status) const;

    int getTaskCountByStatus(QString status);

    Task getTaskByIndex(size_t index) const;

    QString getCalendarUrl() const;

private:
    QScopedPointer<SettingsManager> m_settingsManager;

    QString m_dbPath;
    QList< QSharedPointer<Task> > m_tasks;
    QString m_calendarUrl;

    void loadSettings();
    void loadMockData();

    bool initDb(QString path="");

    QSharedPointer<Task> findTask(size_t index) const;

    size_t getNewTaskIndex();

    void setTasks(const QList< QSharedPointer<Task> > &tasks);

    Task createNewBaseTask();

    void addTask(Task task);

    void removeTask(size_t index);

    void updateTaskStatus(size_t index, QString status);

    void updateTaskInfo(size_t index,
                    QString title,
                    QString description,
                    QDate dueToDate,
                    bool dueToDateEnabled,
                    QTime estimatedTime,
                    QTime actualTime);

    void setDbPath(const QString &dbPath);

    void setCalendarUrl(const QString &calendarUrl);

    friend class Router;

signals:
    void dbPathChanged(QString path);
    void tasksUpdated();
    void calendarUrlChanged(QString url);

public slots:
};

#endif // REPOSITORY_H
