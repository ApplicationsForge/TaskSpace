#ifndef TASK_INDEX_INPUT_WIDGET_H
#define TASK_INDEX_INPUT_WIDGET_H

#include <QWidget>
#include <QVBoxLayout>

#include "libs/qt-material-widgets-master/components/qtmaterialraisedbutton.h"
#include "libs/qt-material-widgets-master/components/qtmaterialflatbutton.h"
#include "libs/qt-material-widgets-master/components/qtmaterialtextfield.h"
#include "libs/qt-material-widgets-master/components/qtmaterialcheckbox.h"

class TaskIndexInputWidget : public QWidget
{
    Q_OBJECT
public:
    explicit TaskIndexInputWidget(QWidget *parent = nullptr);

private:
    QtMaterialTextField* m_indexInput;

signals:
    void indexSelected(size_t index);

public slots:
    void getResult();
};

#endif // TASK_INDEX_INPUT_WIDGET_H
