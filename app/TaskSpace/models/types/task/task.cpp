#include "task.h"

Task::Task(size_t index, QString title, QString status, QString descripion) :
    m_index(index),
    m_title(title),
    m_status(status),
    m_updatedAt(QDateTime::currentDateTime()),
    m_description(descripion),
    m_dueToDate(QDate()),
    m_dueToDateEnabled(false),
    m_estimatedTime(QTime(0, 0)),
    m_actualTime(QTime(0, 0))
{

}

Task::Task(size_t index, const Task &task)
{
    m_index = index;
    this->setActualTime(task.actualTime());
    this->setDescription(task.description());
    this->setDueToDate(task.dueToDate());
    this->setDueToDateEnabled(task.dueToDateEnabled());
    this->setEstimatedTime(task.estimatedTime());
    this->setStatus(task.status());
    this->setTitle(task.title());
}

bool Task::operator<(const Task &task) const
{
    if(this->updatedAt() != task.updatedAt()) {
        return (this->updatedAt() < task.updatedAt());
    }

    return this->index() < task.index();
}

bool Task::operator==(const Task &task) const
{
    return m_index == task.index();
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

QString Task::decoratedBaseInformation() const
{
    QString taskInformation = "[" + QString::number(m_index) + "] "
            + m_title;

    if(m_dueToDateEnabled)
    {
        taskInformation += " (" + m_dueToDate.toString("dd.MM.yyyy") + ")";
        //taskInformation += " " + m_estimatedTime.toString();
        //taskInformation += "/" + m_actualTime.toString() + ")";
    }

    return  taskInformation;
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
