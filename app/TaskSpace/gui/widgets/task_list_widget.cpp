#include "task_list_widget.h"

TaskListWidget::TaskListWidget(QString label, QWidget *parent) :
    QWidget(parent),
    m_label(new QLabel(label, this)),
    m_list(new MyListWidget(this)),
    m_width(300)
{
    this->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
    this->setMinimumWidth(m_width);
    this->setMaximumWidth(m_width);

    m_label->setAlignment(Qt::AlignCenter | Qt::AlignCenter);
    m_label->setFont(QFont("Roboto", 14, QFont::Normal));
    m_label->setStyleSheet("QLabel { background-color: transparent; color: #333; }");

    m_list->setDragEnabled(true);
    m_list->setDropIndicatorShown(true);
    m_list->setDragDropMode(QAbstractItemView::DragDrop);
    m_list->setStyleSheet("QListWidget {} QListWidget::item { color: #333; padding: 10px; }");
    m_list->setAlternatingRowColors(true);

    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
        mainLayout->addWidget(m_label);
        mainLayout->addWidget(m_list);
    this->setLayout(mainLayout);
}

MyListWidget *TaskListWidget::list()
{
    return m_list;
}
