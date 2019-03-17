#ifndef TEAMMATE_H
#define TEAMMATE_H

#include <QString>

class Teammate
{
public:
    Teammate(size_t id, QString name = "Undefined");

    size_t id() const;
    void setId(const size_t &id);

    QString name() const;
    void setName(const QString &name);

private:
    size_t m_id;
    QString m_name;
};

#endif // TEAMMATE_H
