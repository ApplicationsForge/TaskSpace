#ifndef TASK_H
#define TASK_H

#include <QString>
#include <QDate>
#include <QList>

#include "models/types/task/status.h"
#include "models/types/task/teammate.h"
#include "models/types/task/tag.h"

class Task
{
public:
    Task(size_t id,
         QString title,
         Status status,
         QDate dueTo,
         QString description,
         QStringList tags,
         Teammate teammate);

    size_t id() const;

    QString title() const;
    void setTitle(const QString &title);

    Status status() const;
    void setStatus(const Status &status);

    QDate dueTo() const;
    void setDueTo(const QDate &dueTo);

    QString description() const;
    void setDescription(const QString &description);

    QStringList tags() const;
    void setTags(const QStringList &tags);

    Teammate teammate() const;
    void setTeammate(const Teammate &teammate);

private:
    size_t m_id;
    QString m_title;
    Status m_status;
    QDate m_dueTo;
    QString m_description;
    QStringList m_tags;
    Teammate m_teammate;
};

#endif // TASK_H
