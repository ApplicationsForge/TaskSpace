#include "teammate.h"

Teammate::Teammate(size_t id, QString name) :
    m_id(id),
    m_name(name)
{

}

size_t Teammate::id() const
{
    return m_id;
}

void Teammate::setId(const size_t &id)
{
    m_id = id;
}

QString Teammate::name() const
{
    return m_name;
}

void Teammate::setName(const QString &name)
{
    m_name = name;
}
