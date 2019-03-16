#include "my_list_widget_item.h"

MyListWidgetItem::MyListWidgetItem(QString index, QString title, QWidget *parent) :
    MyListWidgetItem(index, title, "", QDate(), QStringList(), QStringList(), parent)
{
}

MyListWidgetItem::MyListWidgetItem(QString index, QString title, QString description, QDate date, QStringList tags, QStringList users, QWidget *parent) :
    QWidget(parent),
    m_index(index),
    m_title(title),
    m_description(description),
    m_date(date),
    m_tags(tags),
    m_users(users),
    m_width(285)
{
    this->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Preferred);
    this->setMinimumWidth(m_width);
    this->setMaximumWidth(m_width);
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    this->layout()->setContentsMargins(5, 15, 5, 10);
        QFrame* container = new QFrame(this);
        QVBoxLayout* containerLayout = new QVBoxLayout(container);
        container->setLayout(containerLayout);
        container->setStyleSheet("margin-left: 1px; margin-right: 1px; background-color: #eee; border: 1px solid #dfdfdf; border-radius: 5px;");
        container->layout()->setSpacing(0);
        container->layout()->setContentsMargins(5, 5, 5, 5);

            // task title
            QLabel* titleLabel = new QLabel(m_title, this);
            titleLabel->setStyleSheet("font-weight: bold; border: 1px solid transparent; background-color: transparent;");
            container->layout()->addWidget(titleLabel);

            // columns container
            QFrame* columnsContainer = new QFrame(container);
            QHBoxLayout* columnsContainerLayout = new QHBoxLayout(columnsContainer);
            columnsContainer->setLayout(columnsContainerLayout);
            columnsContainer->setStyleSheet("margin: 0px; padding: 0px; background:transparent; border: 1px solid transparent");
            columnsContainer->layout()->setSpacing(0);
            columnsContainer->layout()->setContentsMargins(0, 0, 0, 0);

                QFrame* firstColumn = new QFrame(columnsContainer);
                QVBoxLayout* firstColumnLayout = new QVBoxLayout(firstColumn);
                firstColumn->setLayout(firstColumnLayout);
                firstColumn->layout()->setSpacing(0);
                firstColumn->layout()->setContentsMargins(0, 0, 0, 0);
                firstColumn->layout()->setAlignment(Qt::AlignTop);
                    QLabel* dateLabel = new QLabel("Due: " + m_date.toString(), this);
                    dateLabel->setStyleSheet("border: 1px solid transparent; background-color: transparent;");
                    firstColumn->layout()->addWidget(dateLabel);

                    for(auto tag : m_tags)
                    {
                        QLabel* label = new QLabel(tag, this);
                        label->setAlignment(Qt::AlignLeft);
                        label->setStyleSheet("color: #0000bb; border: 1px solid transparent; text-decoration: none;");
                        firstColumn->layout()->addWidget(label);
                    }
                columnsContainer->layout()->addWidget(firstColumn);

                QFrame* secondColumn = new QFrame(columnsContainer);
                QVBoxLayout* secondColumnLayout = new QVBoxLayout(secondColumn);
                secondColumn->setLayout(secondColumnLayout);
                secondColumn->layout()->setSpacing(0);
                secondColumn->layout()->setContentsMargins(0, 0, 0, 0);
                secondColumn->layout()->setAlignment(Qt::AlignTop);
                    for(auto user : m_users)
                    {
                        QLabel* label = new QLabel(user, this);
                        label->setAlignment(Qt::AlignRight);
                        label->setStyleSheet("color: #cd7f32; border: 1px solid transparent;");
                        secondColumn->layout()->addWidget(label);
                    }
                columnsContainer->layout()->addWidget(secondColumn);
            container->layout()->addWidget(columnsContainer);
        mainLayout->addWidget(container);
    this->setLayout(mainLayout);
}

QString MyListWidgetItem::index() const
{
    return m_index;
}

void MyListWidgetItem::setIndex(const QString &index)
{
    m_index = index;
}

QString MyListWidgetItem::title() const
{
    return m_title;
}

void MyListWidgetItem::setTitle(const QString &title)
{
    m_title = title;
}

QString MyListWidgetItem::description() const
{
    return m_description;
}

void MyListWidgetItem::setDescription(const QString &description)
{
    m_description = description;
}

QDate MyListWidgetItem::date() const
{
    return m_date;
}

void MyListWidgetItem::setDate(const QDate &date)
{
    m_date = date;
}

QStringList MyListWidgetItem::tags() const
{
    return m_tags;
}

void MyListWidgetItem::setTags(const QStringList &tags)
{
    m_tags = tags;
}

QStringList MyListWidgetItem::users() const
{
    return m_users;
}

void MyListWidgetItem::setUsers(const QStringList &users)
{
    m_users = users;
}
