#ifndef TASK_H
#define TASK_H

#include <QString>
#include <QDateTime>

class Task
{
public:
    Task(size_t index, QString title, QString status, QString descripion = "");
    Task(size_t index, const Task &task);
    bool operator<(const Task &task) const;
    bool operator==(const Task &task) const;

    size_t index() const;

    QString title() const;
    void setTitle(const QString &title);

    QString status() const;
    void setStatus(const QString &status);

    QString decoratedBaseInformation() const;

    QDateTime updatedAt() const;
    void setUpdatedAt(const QDateTime &updatedAt = QDateTime::currentDateTime());

    QString description() const;
    void setDescription(const QString &description);

    QDate dueToDate() const;
    void setDueToDate(const QDate &dueToDate);

    bool dueToDateEnabled() const;
    void setDueToDateEnabled(bool dueToDateEnabled);

    QTime estimatedTime() const;
    void setEstimatedTime(const QTime &estimatedTime);

    QTime actualTime() const;
    void setActualTime(const QTime &actualTime);

private:
    size_t m_index;
    QString m_title;
    QString m_status;
    QDateTime m_updatedAt;
    QString m_description;
    QDate m_dueToDate;
    bool m_dueToDateEnabled;
    QTime m_estimatedTime;
    QTime m_actualTime;
};

#endif // TASK_H
