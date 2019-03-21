#ifndef TASK_VIEWER_WIDGET_H
#define TASK_VIEWER_WIDGET_H

#include <QWidget>
#include <QVBoxLayout>
#include <QLabel>
#include <QMessageBox>
#include <QCalendarWidget>
#include <QDebug>

#include "libs/qt-material-widgets-master/components/qtmaterialraisedbutton.h"
#include "libs/qt-material-widgets-master/components/qtmaterialflatbutton.h"
#include "libs/qt-material-widgets-master/components/qtmaterialtextfield.h"
#include "libs/qt-material-widgets-master/components/qtmaterialcheckbox.h"

#include <QCheckBox>

#include "libs/qmarkdowntextedit/qmarkdowntextedit.h"

class TaskViewerWidget : public QWidget
{
    Q_OBJECT
public:
    explicit TaskViewerWidget(QWidget *parent = nullptr);

    long taskIndex() const;
    void setTaskIndex(long taskIndex);
    void setEditingEnable(bool enable);

    void setTaskTitle(QString title);
    void setTaskDescription(QString description);
    void setTaskDueToDate(QDate dueToDate);
    void setTaskDueToDateEnabled(bool dueToDateEnabled);

private:
    long m_taskIndex;
    QtMaterialTextField* m_titleWidget;
    QMarkdownTextEdit* m_descriptionWidget;
    QCalendarWidget* m_datePickerWidget;
    QtMaterialCheckBox* m_withoutDateCheckBoxWidget;
    bool m_editingEnable;
signals:
    void taskCreated(QString title, QString description);
    void taskUpdated(size_t index, QString title, QString description);

public slots:
    void saveTaskData();
    void changeEditingEnableStatus();
    void enableEditing();
    void disableEditing();
};

#endif // TASK_VIEWER_WIDGET_H
