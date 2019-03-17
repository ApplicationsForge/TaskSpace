#include "status.h"

Status::Status(size_t id, QString name) :
    m_id(id),
    m_name(name)
{

}

QString Status::name() const
{
    return m_name;
}

void Status::setName(const QString &name)
{
    m_name = name;
}

size_t Status::id() const
{
    return m_id;
}

void Status::setId(const size_t &id)
{
    m_id = id;
}
