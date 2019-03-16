#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>

#include "models/router.h"
#include "libs/qt-material-widgets-master/components/qtmaterialappbar.h"
#include "libs/qt-material-widgets-master/components/qtmaterialiconbutton.h"
#include "libs/qt-material-widgets-master/components/lib/qtmaterialtheme.h"
#include "libs/qt-material-widgets-master/components/qtmaterialraisedbutton.h"


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
    QtMaterialAppBar *const m_appBar;

    void setupWidgets();
};

#endif // MAINWINDOW_H
