#include "task.h"

Task::Task(size_t index, QString title, QString status) :
    m_index(index),
    m_title(title),
    m_status(status)
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
}

QString Task::status() const
{
    return m_status;
}

void Task::setStatus(const QString &status)
{
    m_status = status;
}

QString Task::decoratedBaseInformation()
{
    return "[" + QString::number(m_index) + "] " + m_title + " (Undefined)";
}
