#ifndef TASK_H
#define TASK_H

#include <QString>
#include <QDateTime>

class Task
{
public:
    Task(size_t index, QString title, QString status, QString descripion = "# Description");
    bool operator< (const Task &task) const;

    size_t index() const;

    QString title() const;
    void setTitle(const QString &title);

    QString status() const;
    void setStatus(const QString &status);

    QString decoratedBaseInformation();

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

    bool archived() const;
    void setArchived(bool archived);

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
    bool m_archived;
};

#endif // TASK_H
