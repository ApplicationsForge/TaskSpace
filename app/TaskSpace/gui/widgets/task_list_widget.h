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
    explicit TaskListWidget(QString label = "", QWidget *parent = nullptr);

    MyListWidget* list();

private:
    QLabel* m_label;
    MyListWidget* m_list;
    int m_width;

signals:

public slots:
};

#endif // TASK_LIST_WIDGET_H
