#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QSpacerItem>
#include <QProgressBar>
#include <QTimeEdit>
#include <QDebug>
#include <QDialog>
#include <QScrollArea>
#include <QToolButton>
#include <QFileDialog>
#include <QDialogButtonBox>
#include <QFormLayout>
#include <QWebEngineView>

#include "models/router.h"
#include "libs/qt-material-widgets-master/components/qtmaterialappbar.h"
#include "libs/qt-material-widgets-master/components/qtmaterialiconbutton.h"
#include "libs/qt-material-widgets-master/components/lib/qtmaterialtheme.h"
#include "libs/qt-material-widgets-master/components/qtmaterialraisedbutton.h"
#include "libs/qt-material-widgets-master/components/qtmaterialdrawer.h"
#include "libs/qt-material-widgets-master/components/qtmaterialdialog.h"
#include "libs/qt-material-widgets-master/components/qtmaterialfab.h"
#include "libs/qt-material-widgets-master/components/qtmaterialtextfield.h"
#include "libs/qt-material-widgets-master/components/qtmaterialscrollbar.h"

#include "libs/Qt-custom-gauge-widget-master/source/qcgaugewidget.h"

#include "gui/widgets/task_list_widget.h"
#include "gui/widgets/burndown_chart_widget.h"
#include "gui/widgets/task_status_chart_widget.h"
#include "gui/widgets/mylistwidgetitem.h"
#include "gui/widgets/task_viewer_widget.h"
#include "gui/widgets/task_index_input_widget.h"


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

    QMap<QString, QWidget*> m_widgets;

    QtMaterialAppBar *const m_appBar;
    QtMaterialDrawer *const m_drawer;

    QList<TaskListWidget*> m_taskListWidgets;

    QtMaterialTextField* m_storeDirectoryInput;
    QtMaterialTextField* m_calendarUrlInput;
    QtMaterialTextField* m_avaliableStatusesListInput;

    void setupWidgets();

    void setupAppBar();

    void setupDrawer();

    void setupStatusBar();

    void setupDashboardTab();

    void setupBacklogTab();

    void setupSettingsTab();

    void setupConnections();

    void resetConnections();

    void clearAllTaskLists();

private slots:

    void showDashboardTab();

    void showBacklogTab();

    void showNotesTab();

    void showSettingsTab();

    void showGauge();

    void showFocusTimerDialog();

    void showCalendarDialog();

    void showTaskDialog(Task task, bool newTask = false);

    void onSelectDbButton_clicked();

    void onRouter_TasksUpdated();

    void onAddNewTaskButton_Clicked();

    void onRemoveTaskButton_Clicked();

    void onTaskListWidget_ListWidget_ItemEntered(QListWidgetItem* taskListWidgetItem);

    void onTaskListWidget_TaskDropped(size_t taskIndex, QString status);

    void onTaskViewerWidget_TaskUpdated(size_t index,
                                        QString title,
                                        QString description,
                                        QDate dueToDate,
                                        bool dueToDateEnabled,
                                        QTime estimatedTime,
                                        QTime actualTime);

    void onRemoveTaskInputWidget_IndexSelected(size_t index);

    void onApplySettingsButton_Clicked();
};

#endif // MAINWINDOW_H
