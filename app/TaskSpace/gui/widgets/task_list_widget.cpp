#include "task_list_widget.h"

TaskListWidget::TaskListWidget(size_t statusId, QString statusName, QWidget *parent) :
    QWidget(parent),
    m_statusId(statusId),
    m_statusName(statusName),
    m_label(new QLabel(m_statusName, this)),
    m_list(new MyListWidget(this)),
    m_width(300)
{
    m_label->setAlignment(Qt::AlignCenter | Qt::AlignCenter);
    m_label->setFont(QFont("Roboto", 14, QFont::Normal));
    m_label->setStyleSheet("QLabel { background-color: transparent; color: #333; }");

    this->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
    this->setMinimumWidth(m_width);
    this->setMaximumWidth(m_width);

    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
        mainLayout->addWidget(m_label);
        mainLayout->addWidget(m_list);
    this->setLayout(mainLayout);

    QObject::connect(m_list, SIGNAL(dropAction(QString)), this, SLOT(onMyListWidget_DropAction(QString)));
}

MyListWidget *TaskListWidget::list()
{
    return m_list;
}

size_t TaskListWidget::statusUid() const
{
    return m_statusId;
}

void TaskListWidget::onMyListWidget_DropAction(QString taskTitle)
{
    QRegExp taskIndexRegExp("\\[(\\s)*[0-9]*\\]");

    if(taskIndexRegExp.indexIn(taskTitle) != -1)
    {
        QString matched = taskIndexRegExp.capturedTexts().first();
        matched = matched.remove(QRegExp("\\[|\\]"));

        bool ok = false;
        size_t index = matched.toUInt(&ok);
        if(ok)
        {
            emit this->taskDropped(index, m_statusId);
        }
    }


    /*int lastPos = 0;
    while( ( lastPos = findIndexRegExp.indexIn( taskTitle, lastPos ) ) != -1 ) {
        lastPos += findIndexRegExp.matchedLength();
        findIndexRegExp.c
        qDebug() << re.cap( 0 ) << ":" << re.cap( 1 );
    }*/
}

void TaskListWidget::setTasks(size_t targetStatusId, QList<Task> tasks)
{
    if(targetStatusId != m_statusId)
    {
        return;
    }

    m_list->clear();

    for(auto task : tasks)
    {
        QString title = "[" + QString::number(task.id()) + "] " + task.title() + " (" + task.teammate().name() + ")";
        QListWidgetItem *item = new QListWidgetItem(title);
        m_list->addItem(item);
    }
}
