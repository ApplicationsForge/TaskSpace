#include "task_list_widget.h"

TaskListWidget::TaskListWidget(QString label, QWidget *parent) :
    QWidget(parent),
    m_label(new QLabel(label, this)),
    m_list(new MyListWidget(this)),
    m_width(300)
{
    m_label->setAlignment(Qt::AlignCenter | Qt::AlignCenter);
    m_label->setFont(QFont("Roboto", 14, QFont::Medium));
    m_label->setStyleSheet("QLabel { background-color: transparent; color: #fff; }");

    this->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
    this->setMinimumWidth(m_width);
    this->setMaximumWidth(m_width);

    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(5, 5, 5, 5);
        mainLayout->addWidget(m_label);
        mainLayout->addWidget(m_list);
        mainLayout->addItem(new QSpacerItem(10, 25, QSizePolicy::Expanding, QSizePolicy::Fixed));
    this->setLayout(mainLayout);
}

MyListWidget *TaskListWidget::list()
{
    return m_list;
}
