#ifndef STATUS_H
#define STATUS_H

#include <QString>

class Status
{
public:
    Status(size_t id, QString name = "ToDo");

    QString name() const;
    void setName(const QString &name);

    size_t id() const;
    void setId(const size_t &id);

private:
    size_t m_id;
    QString m_name;
};

#endif // STATUS_H
