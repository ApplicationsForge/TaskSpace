#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_widgets(QMap<QString, QWidget*>()),
    m_appBar(new QtMaterialAppBar(this)),
    m_drawer(new QtMaterialDrawer(this)),
    m_taskListWidgets(QList<TaskListWidget*>())
{
    ui->setupUi(this);

    // cоздаем модель
    Router::getInstance();

    this->setupWidgets();
    this->setupConnections();

    window()->showMaximized();
}

MainWindow::~MainWindow()
{
    this->resetConnections();
    for(auto list : m_taskListWidgets)
    {
        delete list;
    }

    delete ui;
}

void MainWindow::setupWidgets()
{
    // устновка темной темы
    //this->setStyleSheet("background-color: #555;");

    ui->mainWidget->setLayout(new QVBoxLayout(ui->mainWidget));
    this->setupAppBar();
    this->setupDrawer();
    this->setupStatusBar();
    this->setupDashboardTab();
    this->setupBacklogTab();
    this->setupSettingsTab();

    this->showBacklogTab();
    this->onRouter_TasksUpdated();
}

void MainWindow::setupAppBar()
{
    // установка оформления mainToolBar
    QLabel *label = new QLabel("Here is your TaskSpace");
    label->setAttribute(Qt::WA_TranslucentBackground);
    label->setForegroundRole(QPalette::Foreground);
    label->setContentsMargins(6, 0, 0, 0);

    QPalette palette = label->palette();
    palette.setColor(label->foregroundRole(), Qt::white);
    label->setPalette(palette);

    label->setFont(QFont("Roboto", 18, QFont::Normal));

    QtMaterialIconButton *button = new QtMaterialIconButton(QtMaterialTheme::icon("navigation", "menu"));
    button->setIconSize(QSize(24, 24));
    QObject::connect(button, SIGNAL(clicked()), m_drawer, SLOT(openDrawer()));

    m_appBar->appBarLayout()->addWidget(button);
    m_appBar->appBarLayout()->addWidget(label);
    m_appBar->appBarLayout()->addStretch(1);
    m_appBar->setBackgroundColor(QColor("#333"));
    button->setColor(Qt::white);
    button->setFixedWidth(42);

    ui->mainToolBar->addWidget(m_appBar);
    ui->mainToolBar->setFloatable(false);
    ui->mainToolBar->setMovable(false);
    ui->mainToolBar->setStyleSheet("QToolBar { border: 0px; }");
}

void MainWindow::setupDrawer()
{
    // установка оформления меню слева
    QVBoxLayout *drawerLayout = new QVBoxLayout(m_drawer);
    m_drawer->setDrawerLayout(drawerLayout);
    m_drawer->setContentsMargins(10, 0, 0, 0);
    m_drawer->setClickOutsideToClose(true);
    m_drawer->setOverlayMode(true);

    QLabel* menuTitleLabel = new QLabel("Menu", m_drawer);
    menuTitleLabel->setMinimumHeight(30);
    menuTitleLabel->setAlignment(Qt::AlignCenter | Qt::AlignCenter);
    menuTitleLabel->setFont(QFont("Roboto", 16, QFont::Medium));
    menuTitleLabel->setStyleSheet("QLabel { background-color: transparent; color: #333; }");
    drawerLayout->addWidget(menuTitleLabel);

    QtMaterialFlatButton* dashboardButton = new QtMaterialFlatButton("Dashboard", m_drawer);
    dashboardButton->setForegroundColor(QColor("#333"));
    QObject::connect(dashboardButton, SIGNAL(clicked()), this, SLOT(showDashboardTab()));
    drawerLayout->addWidget(dashboardButton);

    QtMaterialFlatButton* backlogButton = new QtMaterialFlatButton("Backlog", m_drawer);
    backlogButton->setForegroundColor(QColor("#333"));
    QObject::connect(backlogButton, SIGNAL(clicked()), this, SLOT(showBacklogTab()));
    drawerLayout->addWidget(backlogButton);

    QtMaterialFlatButton* archiveButton = new QtMaterialFlatButton("Archive", m_drawer);
    archiveButton->setForegroundColor(QColor("#333"));
    //QObject::connect(archiveButton, SIGNAL(clicked()), this, SLOT(showArchiveTab()));
    drawerLayout->addWidget(archiveButton);

    QtMaterialFlatButton* calendarButton = new QtMaterialFlatButton("Calendar", m_drawer);
    calendarButton->setForegroundColor(QColor("#333"));
    QObject::connect(calendarButton, SIGNAL(clicked()), this, SLOT(showCalendarTab()));
    drawerLayout->addWidget(calendarButton);

    QtMaterialFlatButton* notesButton = new QtMaterialFlatButton("Notes", m_drawer);
    notesButton->setForegroundColor(QColor("#333"));
    QObject::connect(notesButton, SIGNAL(clicked()), this, SLOT(showNotesTab()));
    drawerLayout->addWidget(notesButton);

    QtMaterialFlatButton* statisticsButton = new QtMaterialFlatButton("Statistics", m_drawer);
    statisticsButton->setForegroundColor(QColor("#333"));
    //QObject::connect(statisticsButton, SIGNAL(clicked()), this, SLOT(showStatisticsTab()));
    drawerLayout->addWidget(statisticsButton);

    QtMaterialFlatButton* settingsButton = new QtMaterialFlatButton("Settings", m_drawer);
    settingsButton->setForegroundColor(QColor("#333"));
    QObject::connect(settingsButton, SIGNAL(clicked()), this, SLOT(showSettingsTab()));
    drawerLayout->addWidget(settingsButton);


    drawerLayout->addStretch(3);

    QtMaterialFlatButton* closeDrawerButton = new QtMaterialFlatButton("Close", m_drawer);
    closeDrawerButton->setForegroundColor(QColor("#333"));
    QObject::connect(closeDrawerButton, SIGNAL(clicked()), m_drawer, SLOT(closeDrawer()));
    drawerLayout->addWidget(closeDrawerButton);
}

void MainWindow::setupStatusBar()
{
    // установка оформления statusBar
    ui->statusBar->setStyleSheet("background-color: #333; color: #33bb33");
    ui->statusBar->setFont(QFont("Consolas", 14));
    ui->statusBar->showMessage(tr("State: ready 0123456789"));
}

void MainWindow::setupDashboardTab()
{
    QWidget *container = new QWidget(ui->mainWidget);
    container->setObjectName("dashboardContainerWidget");
    m_widgets.insert(container->objectName(), container);
    QVBoxLayout *containerLayout = new QVBoxLayout(container);
        QLabel *dashboardTitleLabel = new QLabel("Dashboard", container);
        dashboardTitleLabel->setAlignment(Qt::AlignCenter | Qt::AlignCenter);
        dashboardTitleLabel->setFont(QFont("Roboto", 18, QFont::Normal));
        dashboardTitleLabel->setStyleSheet("QLabel { background-color: transparent; color: #333; }");
        containerLayout->addWidget(dashboardTitleLabel);

        QWidget *subContainer = new QWidget(container);
            QHBoxLayout *subContainerLayout = new QHBoxLayout(subContainer);
                QWidget *chartsContainerWidget = new QWidget(subContainer);
                chartsContainerWidget->setLayout(new QVBoxLayout(chartsContainerWidget));
                    /*QLabel *burndownChartLabel = new QLabel("Burndown Chart", chartsContainerWidget);
                    burndownChartLabel->setAlignment(Qt::AlignCenter | Qt::AlignCenter);
                    burndownChartLabel->setFont(QFont("Roboto", 16, QFont::Normal));
                    chartsContainerWidget->layout()->addWidget(burndownChartLabel);

                    BurndownChartWidget* burndownChartWidget = new BurndownChartWidget(chartsContainerWidget);
                    burndownChartWidget->setObjectName("burndownChartWidget");
                    m_widgets.insert(burndownChartWidget->objectName(), burndownChartWidget);
                    chartsContainerWidget->layout()->addWidget(burndownChartWidget);*/

                    TaskStatusChartWidget* taskStatusChartWidget = new TaskStatusChartWidget(chartsContainerWidget);
                    taskStatusChartWidget->setObjectName("taskStatusChartWidget");
                    m_widgets.insert(taskStatusChartWidget->objectName(), taskStatusChartWidget);
                    chartsContainerWidget->layout()->addWidget(taskStatusChartWidget);


                    QWidget *actionsContainerWidget = new QWidget(chartsContainerWidget);
                        QHBoxLayout *actionsContainerWidgetLayout = new QHBoxLayout(actionsContainerWidget);
                            actionsContainerWidgetLayout->addWidget(new QtMaterialRaisedButton("Export", actionsContainerWidget));
                            actionsContainerWidgetLayout->addWidget(new QtMaterialRaisedButton("History", actionsContainerWidget));
                            actionsContainerWidgetLayout->addItem(new QSpacerItem(10, 10, QSizePolicy::Expanding, QSizePolicy::Fixed));
                        actionsContainerWidget->setLayout(actionsContainerWidgetLayout);
                    chartsContainerWidget->layout()->addWidget(actionsContainerWidget);
                subContainerLayout->addWidget(chartsContainerWidget);
                subContainerLayout->setStretch(0, 4);

                QWidget *toolsContainerWidget = new QWidget(subContainer);
                QVBoxLayout *toolsFrameLayout = new QVBoxLayout(toolsContainerWidget);
                toolsContainerWidget->setLayout(toolsFrameLayout);
                    QLabel *toolsTitleLabel = new QLabel("Tools", toolsContainerWidget);
                    toolsTitleLabel->setAlignment(Qt::AlignCenter | Qt::AlignCenter);
                    toolsTitleLabel->setFont(QFont("Roboto", 16, QFont::Normal));
                    toolsContainerWidget->layout()->addWidget(toolsTitleLabel);

                    QtMaterialRaisedButton *focusTimerButton = new QtMaterialRaisedButton("Focus Timer", toolsContainerWidget);
                    QObject::connect(focusTimerButton, SIGNAL(clicked()), this, SLOT(showFocusTimerDialog()));
                    toolsFrameLayout->addWidget(focusTimerButton);

                    toolsFrameLayout->layout()->addItem(new QSpacerItem(10, 10, QSizePolicy::Expanding, QSizePolicy::Expanding));
                subContainerLayout->addWidget(toolsContainerWidget);
                subContainerLayout->setStretch(1, 1);
            subContainer->setLayout(subContainerLayout);
         containerLayout->addWidget(subContainer);
    container->setLayout(containerLayout);
    ui->mainWidget->layout()->addWidget(container);
    container->hide();
}

void MainWindow::setupBacklogTab()
{
    Router &router = Router::getInstance();

    QWidget* container = new QWidget(ui->mainWidget);
    container->setObjectName("backlogContainerWidget");
    m_widgets.insert(container->objectName(), container);
    QVBoxLayout *containerLayout = new QVBoxLayout(container);
    containerLayout->setSpacing(0);
        QLabel *backlogTitleLabel = new QLabel("Backlog", container);
        backlogTitleLabel->setAlignment(Qt::AlignCenter | Qt::AlignCenter);
        backlogTitleLabel->setFont(QFont("Roboto", 18, QFont::Normal));
        backlogTitleLabel->setStyleSheet("QLabel { background-color: transparent; color: #333; }");
        containerLayout->addWidget(backlogTitleLabel);

        QWidget *actionsContainer = new QWidget(container);
        actionsContainer->setLayout(new QHBoxLayout(actionsContainer));
        actionsContainer->setContentsMargins(0, 0, 0, 0);
            QtMaterialRaisedButton *addNewTaskButton = new QtMaterialRaisedButton("Add New Task", actionsContainer);
            QObject::connect(addNewTaskButton, SIGNAL(clicked()), this, SLOT(onAddNewTaskButton_Clicked()));
            actionsContainer->layout()->addWidget(addNewTaskButton);
            actionsContainer->layout()->addWidget(new QtMaterialRaisedButton("Remove Task", actionsContainer));
            actionsContainer->layout()->addItem(new QSpacerItem(10, 10, QSizePolicy::Expanding, QSizePolicy::Fixed));
            actionsContainer->layout()->addWidget(new QtMaterialRaisedButton("Sync Tasks With Trello", actionsContainer));
        containerLayout->addWidget(actionsContainer);

        QScrollArea *scrollArea = new QScrollArea(container);
        scrollArea->setWidgetResizable(true);
        scrollArea->setStyleSheet("QScrollArea { border: 0px; }");
        scrollArea->setContentsMargins(0, 0, 0, 0);
            QWidget *scrollAreaContent = new QWidget(scrollArea);
            scrollAreaContent->setLayout(new QHBoxLayout(scrollAreaContent));
            scrollAreaContent->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
            scrollAreaContent->setContentsMargins(0, 0, 0, 10);
                QStringList avaliableStatuses = router.getRepository()->getAvaliableStatuses();
                for(auto status : avaliableStatuses)
                {
                    TaskListWidget* taskListWidget = new TaskListWidget(status, scrollAreaContent);
                    taskListWidget->setObjectName(status + "TaskListWidget");
                    QObject::connect(taskListWidget, SIGNAL(taskDropped(size_t, QString)), &router, SLOT(onTaskListWidget_TaskDropped(size_t, QString)));
                    //m_widgets.insert(taskListWidget->objectName(), taskListWidget);
                    m_taskListWidgets.append(taskListWidget);
                    scrollAreaContent->layout()->addWidget(taskListWidget);
                }
            scrollArea->setWidget(scrollAreaContent);
        containerLayout->addWidget(scrollArea);
    container->setLayout(containerLayout);
    ui->mainWidget->layout()->addWidget(container);
    container->hide();
}

void MainWindow::setupSettingsTab()
{
    Router &router = Router::getInstance();
    QString dbPath = router.getRepository()->dbPath();

    QWidget* container = new QWidget(ui->mainWidget);
    container->setObjectName("settingsContainerWidget");
    m_widgets.insert(container->objectName(), container);
    QVBoxLayout *containerLayout = new QVBoxLayout(container);
        QLabel *settingsTitleLabel = new QLabel("Settings", container);
        settingsTitleLabel->setAlignment(Qt::AlignCenter | Qt::AlignCenter);
        settingsTitleLabel->setFont(QFont("Roboto", 18, QFont::Normal));
        settingsTitleLabel->setStyleSheet("QLabel { background-color: transparent; color: #333; }");
        containerLayout->addWidget(settingsTitleLabel);

        QWidget *dbPathWidget = new QWidget(container);
        dbPathWidget->setLayout(new QHBoxLayout(dbPathWidget));
            QLabel *dbPathTitleLabel = new QLabel("Database Path:", dbPathWidget);
            dbPathWidget->layout()->addWidget(dbPathTitleLabel);

            QLabel *dbPathValueLabel = new QLabel(dbPath, dbPathWidget);
            QObject::connect(router.getRepository(), SIGNAL(dbPathChanged(QString)), dbPathValueLabel, SLOT(setText(QString)));
            dbPathWidget->layout()->addWidget(dbPathValueLabel);

            QToolButton* selectDbToolButton = new QToolButton(dbPathWidget);
            selectDbToolButton->setText("...");
            QObject::connect(selectDbToolButton, SIGNAL(clicked()), this, SLOT(onSelectDbToolButton_clicked()));
            dbPathWidget->layout()->addWidget(selectDbToolButton);

            dbPathWidget->layout()->addItem(new QSpacerItem(10, 10, QSizePolicy::Expanding, QSizePolicy::Fixed));
        containerLayout->addWidget(dbPathWidget);
        containerLayout->layout()->addItem(new QSpacerItem(10, 10, QSizePolicy::Expanding, QSizePolicy::Expanding));
    container->setLayout(containerLayout);
    ui->mainWidget->layout()->addWidget(container);
    container->hide();
}

void MainWindow::setupConnections()
{
    Router &router = Router::getInstance();
    QObject::connect(&router, SIGNAL(tasksUpdated()), this, SLOT(onRouter_TasksUpdated()));
}

void MainWindow::resetConnections()
{
    Router &router = Router::getInstance();
    QObject::disconnect(&router, SIGNAL(tasksUpdated()), this, SLOT(onRouter_TasksUpdated()));
}

void MainWindow::clearAllTaskLists()
{
    for(auto taskListWidget : m_taskListWidgets)
    {
        taskListWidget->list()->clear();
    }
}

void MainWindow::showDashboardTab()
{
    m_widgets["dashboardContainerWidget"]->show();
    m_widgets["backlogContainerWidget"]->hide();
    m_widgets["settingsContainerWidget"]->hide();
}

void MainWindow::showBacklogTab()
{
    m_widgets["dashboardContainerWidget"]->hide();
    m_widgets["backlogContainerWidget"]->show();
    m_widgets["settingsContainerWidget"]->hide();
}

void MainWindow::showCalendarTab()
{
    /*qDeleteAll(ui->mainFrame->children());
    ui->mainFrame->setLayout(new QHBoxLayout(ui->mainFrame));
    ui->mainFrame->layout()->addWidget(new QtMaterialRaisedButton("Calendar", ui->mainFrame));*/
}

void MainWindow::showNotesTab()
{
    /*qDeleteAll(ui->mainFrame->children());
    ui->mainFrame->setLayout(new QHBoxLayout(ui->mainFrame));
    ui->mainFrame->layout()->addWidget(new QtMaterialRaisedButton("Notes", ui->mainFrame));

    this->showGauge();*/
}

void MainWindow::showSettingsTab()
{
    m_widgets["dashboardContainerWidget"]->hide();
    m_widgets["backlogContainerWidget"]->hide();
    m_widgets["settingsContainerWidget"]->show();
}

void MainWindow::showGauge()
{
    QcGaugeWidget* speedGauge = new QcGaugeWidget(this);
    QcBackgroundItem *bkg = speedGauge->addBackground(50);
    bkg->clearrColors();
    bkg->addColor(float(0.1), Qt::black);

    QcLabelItem *lab = speedGauge->addLabel(30);
    lab->setColor(Qt::blue);
    lab->setText("0");

    QcNeedleItem *speedNeedle;
    speedNeedle = speedGauge->addNeedle(45);
    speedNeedle->setLabel(lab);
    speedNeedle->setColor(Qt::blue);
    speedNeedle->setValueRange(0,100);
    speedGauge->addBackground(3);

    //ui->mainFrame->layout()->addWidget(speedGauge);
}

void MainWindow::showFocusTimerDialog()
{
    QDialog *focusTimerDialog = new QDialog(this);
    focusTimerDialog->setWindowTitle("Focus Timer Dialog");
    focusTimerDialog->setMinimumSize(400, 300);
    focusTimerDialog->show();
}

void MainWindow::onSelectDbToolButton_clicked()
{
    Router& router = Router::getInstance();
    QString path = QFileDialog::getOpenFileName(this, "Select Database", "", "*.db");
    if(path.length() > 0)
    {
        router.getRepository()->setDbPath(path);
    }
}

void MainWindow::onRouter_TasksUpdated()
{
    //qDebug() << "MainWindow::onRouter_TasksUpdated";
    Router& router = Router::getInstance();
    for(auto taskListWidget : m_taskListWidgets)
    {
        taskListWidget->list()->clear();
        QList<Task> tasks = router.getRepository()->getTasks(taskListWidget->status());
        for(auto task : tasks)
        {
            QListWidgetItem *item = new QListWidgetItem();
            item->setText(task.decoratedBaseInformation());
            taskListWidget->list()->addItem(item);
        }
    }

    QWidget* rawTaskStatusBarWidget = m_widgets["taskStatusChartWidget"];
    TaskStatusChartWidget* taskStatusChartWidget = qobject_cast<TaskStatusChartWidget*>(rawTaskStatusBarWidget);
    if(taskStatusChartWidget != nullptr)
    {
        QStringList avaliableStatuses = router.getRepository()->getAvaliableStatuses();
        QList<int> taskCountByStatus;
        for(auto status : avaliableStatuses)
        {
            taskCountByStatus << router.getRepository()->getTaskCountByStatus(status);
        }
        taskStatusChartWidget->updateChartWidget(taskCountByStatus, avaliableStatuses);
    }
}

void MainWindow::onAddNewTaskButton_Clicked()
{
    Router& router = Router::getInstance();
    router.addExampleTask();
}
