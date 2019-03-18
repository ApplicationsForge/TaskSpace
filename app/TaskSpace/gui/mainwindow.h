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

#include "models/router.h"
#include "libs/qt-material-widgets-master/components/qtmaterialappbar.h"
#include "libs/qt-material-widgets-master/components/qtmaterialiconbutton.h"
#include "libs/qt-material-widgets-master/components/lib/qtmaterialtheme.h"
#include "libs/qt-material-widgets-master/components/qtmaterialraisedbutton.h"
#include "libs/qt-material-widgets-master/components/qtmaterialdrawer.h"
#include "libs/qt-material-widgets-master/components/qtmaterialdialog.h"
#include "libs/qt-material-widgets-master/components/qtmaterialfab.h"
#include "libs/qt-material-widgets-master/components/qtmaterialtextfield.h"

#include "libs/Qt-custom-gauge-widget-master/source/qcgaugewidget.h"

#include "gui/widgets/task_list_widget.h"
//#include "gui/widgets/burndown_chart_widget.h"

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

    void showCalendarTab();

    void showNotesTab();

    void showSettingsTab();

    void showGauge();

    void showFocusTimerDialog();

    void onSelectDbToolButton_clicked();
    void onRouter_TasksUpdated();
    void onAddNewTaskButton_Clicked();
};

#endif // MAINWINDOW_H
