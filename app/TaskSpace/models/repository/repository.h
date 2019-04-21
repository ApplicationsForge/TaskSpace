#ifndef REPOSITORY_H
#define REPOSITORY_H

#include <QObject>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QtDebug>

#include "libs/json_parser/json.h"

#include "models/settings_manager/settings_manager.h"
#include "models/types/task/task.h"
#include "models/utils/file_reader.h"

class Router;

class Repository : public QObject
{
    Q_OBJECT
public:
    explicit Repository(QObject *parent = nullptr);

    ~Repository();

    QList<Task> getTasks(const QString &filter="") const;
    QList<Task> getTasksByStatus(const QString &status, const QString &filter="") const;
    int getTaskCountByStatus(QString status);
    Task& getActiveTaskByIndex(size_t index);
    Task getArchivedTaskByIndex(size_t index);
    Task& createNewBaseTask();
    void addTask(const Task &task);
    void removeTask(size_t index);
    void updateTaskStatus(size_t index, const QString &status);
    void updateTaskInfo(size_t index,
                    const QString &title,
                    const QString &description,
                    const QDate &dueToDate,
                    bool dueToDateEnabled,
                    const QTime &estimatedTime,
                    const QTime &actualTime);

    QString storeDirectory() const;
    void setStoreDirectory(const QString &storeDirectory);

    QString getCalendarUrl() const;
    void setCalendarUrl(const QString &calendarUrl);

    QStringList getAvaliableStatuses();
    void setAvaliableStatuses(const QStringList &avaliableStatuses);

    QList<Task> getArchivedTasks();
    void archiveTask(size_t index);
    void archiveTask(const Task &task);
    void archiveTasksByStatus(QString status);
    void unarchiveTask(const Task &task, QString status);

private:
    QScopedPointer<SettingsManager> m_settingsManager;

    QString m_storeDirectory;

    QStringList m_avaliableStatuses;
    QList<Task> m_tasks;
    QString m_calendarUrl;

    void loadSettings();

    void syncTasks();

    static QList<Task> loadTasks(const QString &filePath);

    static void saveTasks(const QList<Task> &tasks, const QString &storeDirectory);

    static QtJson::JsonArray convertTaskListToJson(const QList<Task> &tasks);

    static QList<Task> convertTaskJsonToList(const QtJson::JsonArray &taskJsonArray);

    static QString resolveTaskFilePath(const QString &storeDirectory);

    static QString resolveArchiveFilePath(const QString &storeDirectory);

    static QList<Task> filterTasks(const QList<Task> &tasks, const QString &filter = "");

    static Task& findTask(QList<Task> &tasks, size_t index);

    size_t getNewTaskIndex();

    void setTasks(const QList<Task> &tasks);

    friend class Router;

signals:
    void storeDirectoryChanged(QString path);
    void tasksUpdated();
    void calendarUrlChanged(QString url);
    void avaliableStatusesChanged(QStringList statuses);

public slots:
};

#endif // REPOSITORY_H
