#include "tag.h"

Tag::Tag(QString name) :
    m_name(name)
{

}

  QString Tag::name() const
{
    return m_name;
}

void Tag::setName(const QString &name)
{
    m_name = name;
}
