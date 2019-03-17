#include "task.h"

Task::Task(size_t index,
           QString title,
           Status status,
           QDate dueTo,
           QString description,
           QStringList tags,
           Teammate teammate) :
    m_index(index),
    m_title(title),
    m_status(status),
    m_dueTo(dueTo),
    m_description(description),
    m_tags(tags),
    m_teammate(teammate)
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

Status Task::status() const
{
    return m_status;
}

void Task::setStatus(const Status &status)
{
    m_status = status;
}

QDate Task::dueTo() const
{
    return m_dueTo;
}

void Task::setDueTo(const QDate &dueTo)
{
    m_dueTo = dueTo;
}

QString Task::description() const
{
    return m_description;
}

void Task::setDescription(const QString &description)
{
    m_description = description;
}

QStringList Task::tags() const
{
    return m_tags;
}

void Task::setTags(const QStringList &tags)
{
    m_tags = tags;
}

Teammate Task::teammate() const
{
    return m_teammate;
}

void Task::setTeammate(const Teammate &teammate)
{
    m_teammate = teammate;
}
