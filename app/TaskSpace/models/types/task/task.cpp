#include "task.h"

Task::Task(size_t index, QString title, QString status, QString descripion) :
    m_index(index),
    m_title(title),
    m_status(status),
    m_updatedAt(QDateTime::currentDateTime()),
    m_description(descripion),
    m_dueToDate(QDate()),
    m_dueToDateEnabled(true),
    m_estimatedTime(QTime(0, 0)),
    m_actualTime(QTime(0, 0)),
    m_archived(false)
{

}

bool Task::operator<(const Task &task) const
{
    if(this->updatedAt() != task.updatedAt()) {
        return (this->updatedAt() < task.updatedAt());
    }

    return this->index() < task.index();
}

size_t Task::index() const
{
    return m_index;
}

QString Task::title() const
{
    return m_title;
}

void Task::setTitle(const QString &title)
{
    m_title = title;
    this->setUpdatedAt(QDateTime::currentDateTime());
}

QString Task::status() const
{
    return m_status;
}

void Task::setStatus(const QString &status)
{
    m_status = status;
    this->setUpdatedAt(QDateTime::currentDateTime());
}

QString Task::decoratedBaseInformation()
{
    return "[" + QString::number(m_index) + "] " + m_title + " (Undefined)";
}

QDateTime Task::updatedAt() const
{
    return m_updatedAt;
}

void Task::setUpdatedAt(const QDateTime &updatedAt)
{
    m_updatedAt = updatedAt;
}

QString Task::description() const
{
    return m_description;
}

void Task::setDescription(const QString &description)
{
    m_description = description;
}

QDate Task::dueToDate() const
{
    return m_dueToDate;
}

void Task::setDueToDate(const QDate &dueToDate)
{
    m_dueToDate = dueToDate;
}

bool Task::dueToDateEnabled() const
{
    return m_dueToDateEnabled;
}

void Task::setDueToDateEnabled(bool dueToDateEnabled)
{
    m_dueToDateEnabled = dueToDateEnabled;
}

QTime Task::estimatedTime() const
{
    return m_estimatedTime;
}

void Task::setEstimatedTime(const QTime &estimatedTime)
{
    m_estimatedTime = estimatedTime;
}

QTime Task::actualTime() const
{
    return m_actualTime;
}

void Task::setActualTime(const QTime &actualTime)
{
    m_actualTime = actualTime;
}

bool Task::archived() const
{
    return m_archived;
}

void Task::setArchived(bool archived)
{
    m_archived = archived;
}
