#ifndef TASK_LIST_WIDGET_H
#define TASK_LIST_WIDGET_H

#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>

#include "gui/widgets/my_list_widget.h"
#include "models/types/task/task.h"

class TaskListWidget : public QWidget
{
    Q_OBJECT
public:
    explicit TaskListWidget(size_t statusId, QString statusName = "", QWidget *parent = nullptr);

    MyListWidget* list();

    size_t statusUid() const;

private:
    size_t m_statusId;
    QString m_statusName;
    QLabel* m_label;
    MyListWidget* m_list;
    int m_width;

signals:
    void taskDropped(size_t taskUid, size_t statusUid);

public slots:
    void onMyListWidget_DropAction(QString taskTitle);
    void setTasks(size_t targetStatusId, QList<Task> tasks);
};

#endif // TASK_LIST_WIDGET_H
