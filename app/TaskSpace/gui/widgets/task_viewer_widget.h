#ifndef TASK_VIEWER_WIDGET_H
#define TASK_VIEWER_WIDGET_H

#include <QWidget>
#include <QVBoxLayout>
#include <QLabel>
#include <QMessageBox>
#include <QCalendarWidget>
#include <QDebug>
#include <QTimeEdit>

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

    void setTaskTitle(const QString &title);
    void setTaskDescription(const QString &description);
    void setTaskDueToDate(const QDate &dueToDate);
    void setTaskDueToDateEnabled(bool dueToDateEnabled);
    void setTaskEstimatedTime(const QTime &estimatedTime);
    void setTaskActualTime(const QTime &actualTime);

private:
    long m_taskIndex;
    QtMaterialTextField* m_titleWidget;
    QMarkdownTextEdit* m_descriptionWidget;
    QCalendarWidget* m_datePickerWidget;
    QtMaterialCheckBox* m_withoutDateCheckBoxWidget;
    QTimeEdit* m_estimatedTimeWidget;
    QTimeEdit* m_actualTimeWidget;
    bool m_editingEnable;
signals:
    void taskUpdated(size_t index,
                     QString title,
                     QString description,
                     QDate dueToDate,
                     bool withoutDueToDate,
                     QTime estimatedTime,
                     QTime actualTime);

public slots:
    void saveTaskData();
    void changeEditingEnableStatus();
    void enableEditing();
    void disableEditing();
};

#endif // TASK_VIEWER_WIDGET_H
