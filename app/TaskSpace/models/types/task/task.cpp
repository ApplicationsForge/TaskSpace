#include "task.h"

Task::Task(size_t index, QString title, QString status) :
    m_index(index),
    m_title(title),
    m_status(status),
    m_updatedAt(QDateTime::currentDateTime())
{

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

bool Task::operator<(const Task &task) const
{
    if(this->updatedAt() != task.updatedAt()) {
        return (this->updatedAt() < task.updatedAt());
    }

    return this->index() < task.index();
}
