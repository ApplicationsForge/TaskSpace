#ifndef TASK_LIST_WIDGET_H
#define TASK_LIST_WIDGET_H

#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>

#include "gui/widgets/my_list_widget.h"

class TaskListWidget : public QWidget
{
    Q_OBJECT
public:
    explicit TaskListWidget(QString status = "", QWidget *parent = nullptr);

    MyListWidget* list();

private:
    QString m_status;
    QLabel* m_label;
    MyListWidget* m_list;
    int m_width;

signals:
    void taskDropped(size_t taskIndex, QString status);

public slots:
    void onMyListWidget_DropAction(QString data);
};

#endif // TASK_LIST_WIDGET_H
