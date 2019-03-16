#include "task_list_widget.h"

TaskListWidget::TaskListWidget(QString label, QWidget *parent) :
    QWidget(parent),
    m_label(new QLabel(label, this)),
    m_list(new MyListWidget(this)),
    m_size(300)
{
    m_label->setAlignment(Qt::AlignCenter | Qt::AlignCenter);
    m_label->setFont(QFont("Roboto", 14, QFont::Medium));
    m_label->setStyleSheet("QLabel { background-color: transparent; color: #fff; }");

    QVBoxLayout* mainLayout = new QVBoxLayout(this);

    mainLayout->addWidget(m_label);
    mainLayout->addWidget(m_list);

    this->setLayout(mainLayout);
    this->layout()->setContentsMargins(5, 5, 5, 5);
    this->setMinimumWidth(m_size);
    this->setMaximumWidth(m_size);
}
