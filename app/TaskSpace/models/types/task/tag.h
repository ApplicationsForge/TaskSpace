#ifndef TAG_H
#define TAG_H

#include <QString>

class Tag
{
public:
    Tag(QString name);

    QString name() const;
    void setName(const QString &name);

private:
    QString m_name;
};

#endif // TAG_H
