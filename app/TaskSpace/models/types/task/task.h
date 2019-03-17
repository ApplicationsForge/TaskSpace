#ifndef TASK_H
#define TASK_H

#include <QString>

class Task
{
public:
    Task(size_t index, QString title, QString status);

    size_t index() const;

    QString title() const;
    void setTitle(const QString &title);

    QString status() const;
    void setStatus(const QString &status);

    QString decoratedBaseInformation();

private:
    size_t m_index;
    QString m_title;
    QString m_status;
};

#endif // TASK_H
