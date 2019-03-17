#ifndef TASK_H
#define TASK_H

#include <QString>
#include <QDate>

class Task
{
public:
    Task();

private:
    size_t m_index;
    QString m_title;
    QString m_status;
    QDate m_dueTo;
    QString m_description;
    QStringList m_tags;
    QStringList m_users;
};

#endif // TASK_H
