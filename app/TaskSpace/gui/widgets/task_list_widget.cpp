#include "task_list_widget.h"

TaskListWidget::TaskListWidget(QString status, QWidget *parent) :
    QWidget(parent),
    m_status(status),
    m_label(new QLabel(m_status, this)),
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
    m_list->setDragDropOverwriteMode(true);
    m_list->setDropIndicatorShown(true);
    m_list->setStyleSheet("QListWidget {} QListWidget::item { color: #333; padding: 10px; }");
    m_list->setAlternatingRowColors(true);
    QObject::connect(m_list, SIGNAL(dropAction(QString)), this, SLOT(onMyListWidget_DropAction(QString)));

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

QString TaskListWidget::status() const
{
    return m_status;
}

void TaskListWidget::onMyListWidget_DropAction(QString data)
{
    qDebug() << "TaskListWidget::onMyListWidget_DropAction" << data;
    QRegExp taskIndexRegExp("\\[(\\s)*[0-9]*\\]");

    if(taskIndexRegExp.indexIn(data) != -1)
    {
        QString matched = taskIndexRegExp.capturedTexts().first();
        matched = matched.remove(QRegExp("\\[|\\]"));

        bool ok = false;
        size_t index = matched.toUInt(&ok);
        //qDebug() << matched << index << m_status;
        if(ok)
        {
            //qDebug() << "test";
            emit this->taskDropped(index, m_status);
        }
    }
}
