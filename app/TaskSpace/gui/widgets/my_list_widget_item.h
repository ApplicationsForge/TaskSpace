#ifndef MY_LIST_WIDGET_ITEM_H
#define MY_LIST_WIDGET_ITEM_H

#include <QWidget>
#include <QListWidgetItem>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QSpacerItem>
#include <QTextEdit>
#include <QDebug>
#include <QDate>

class MyListWidgetItem : public QWidget
{
    Q_OBJECT
public:
    explicit MyListWidgetItem(QString index, QString title, QWidget *parent = nullptr);
    explicit MyListWidgetItem(QString index, QString title, QString description = "", QDate date = QDateTime::currentDateTime().date(), QStringList tags = QStringList(), QStringList users = QStringList(), QWidget *parent = nullptr);

    QString index() const;
    void setIndex(const QString &index);

    QString title() const;
    void setTitle(const QString &title);

    QString description() const;
    void setDescription(const QString &description);

    QDate date() const;
    void setDate(const QDate &date);

    QStringList tags() const;
    void setTags(const QStringList &tags);

    QStringList users() const;
    void setUsers(const QStringList &users);

private:
    QString m_index;
    QString m_title;
    QString m_description;
    QDate m_date;
    QStringList m_tags;
    QStringList m_users;
    int m_width;

signals:

public slots:
};

#endif // MY_LIST_WIDGET_ITEM_H
