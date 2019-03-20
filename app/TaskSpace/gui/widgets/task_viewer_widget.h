#ifndef TASK_VIEWER_WIDGET_H
#define TASK_VIEWER_WIDGET_H

#include <QWidget>
#include <QVBoxLayout>
#include <QLabel>

#include "libs/qt-material-widgets-master/components/qtmaterialraisedbutton.h"
#include "libs/qt-material-widgets-master/components/qtmaterialflatbutton.h"
#include "libs/qt-material-widgets-master/components/qtmaterialtextfield.h"

#include "libs/qmarkdowntextedit/qmarkdowntextedit.h"

class TaskViewerWidget : public QWidget
{
    Q_OBJECT
public:
    explicit TaskViewerWidget(QWidget *parent = nullptr);

    void setTaskTitle(QString title);
    void setInputLocked(bool inputLocked);

private:
    QtMaterialTextField* m_titleTextField;
    QMarkdownTextEdit* m_descriptionTextEdit;
    bool m_inputLocked;

signals:
    void taskCreated(QString title, QString description);
    void taskUpdated(size_t index, QString title, QString description);

public slots:
    void changeLockStatus();
    void saveTaskData();
};

#endif // TASK_VIEWER_WIDGET_H
