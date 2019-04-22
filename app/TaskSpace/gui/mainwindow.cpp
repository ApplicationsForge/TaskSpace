#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_widgets(QMap<QString, QWidget*>()),
    m_appBar(new QtMaterialAppBar(this)),
    m_drawer(new QtMaterialDrawer(this)),
    m_activeTaskListWidgets(QList<TaskListWidget*>()),
    m_archivedTaskListWidget(new MyListWidget(this)),
    m_storeDirectoryInput(new QLineEdit(this)),
    m_calendarUrlInput(new QLineEdit(this)),
    m_avaliableStatusesListInput(new QLineEdit(this)),
    m_filterInput(new QLineEdit(this))
{
    ui->setupUi(this);
    // cоздаем модель
    try
    {
        Router::getInstance();
    }
    catch(...)
    {
        QMessageBox(QMessageBox::Critical, "Error", "An error has occured during application initialisation.").exec();
        QApplication::exit(0);
    }

    this->setupWidgets();
    this->setupConnections();

    window()->showMaximized();
}

MainWindow::~MainWindow()
{
    this->resetConnections();
    for(auto list : m_activeTaskListWidgets)
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
    ui->mainWidget->layout()->setContentsMargins(0, 0, 0, 0);

    this->setupAppBar();
    this->setupDrawer();
    this->setupStatusBar();
    this->setupDashboardTab();
    this->setupBacklogTab();
    this->setupArchiveTab();
    this->setupSettingsTab();

    this->showBacklogTab();
    //this->showDashboardTab();
    this->onRouter_TasksUpdated();
}

void MainWindow::setupAppBar()
{
    // установка оформления mainToolBar
    QLabel *label = new QLabel("Here is your TaskSpace", this);
    label->setAttribute(Qt::WA_TranslucentBackground);
    label->setForegroundRole(QPalette::Foreground);
    label->setContentsMargins(6, 0, 0, 0);

    QPalette palette = label->palette();
    palette.setColor(label->foregroundRole(), Qt::white);
    label->setPalette(palette);

    label->setFont(QFont("Roboto", 18, QFont::Normal));

    QtMaterialIconButton *menuButton = new QtMaterialIconButton(QtMaterialTheme::icon("navigation", "menu"), this);
    menuButton->setIconSize(QSize(24, 24));
    menuButton->setColor(Qt::white);
    menuButton->setFixedWidth(42);
    QObject::connect(menuButton, SIGNAL(clicked()), m_drawer, SLOT(openDrawer()));

    m_appBar->appBarLayout()->addWidget(menuButton);
    m_appBar->appBarLayout()->addWidget(label);
    m_appBar->appBarLayout()->addStretch(1);
    m_appBar->setBackgroundColor(QColor("#333"));

    QtMaterialIconButton *calendarButton = new QtMaterialIconButton(QtMaterialTheme::icon("action", "event"), this);
    calendarButton->setIconSize(QSize(24, 24));
    calendarButton->setColor(Qt::white);
    calendarButton->setFixedWidth(42);
    QObject::connect(calendarButton, SIGNAL(pressed()), this, SLOT(showCalendarDialog()));
    m_appBar->appBarLayout()->addWidget(calendarButton);

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

    QPushButton* dashboardButton = new QPushButton("Dashboard", m_drawer);
    dashboardButton->setFlat(true);
    QObject::connect(dashboardButton, &QPushButton::clicked, this, [=](){
        this->showDashboardTab();
    });
    drawerLayout->addWidget(dashboardButton);

    QPushButton* backlogButton = new QPushButton("Backlog", m_drawer);
    backlogButton->setFlat(true);
    //backlogButton->setForegroundColor(QColor("#333"));
    QObject::connect(backlogButton, &QPushButton::clicked, this, [=](){
        this->showBacklogTab();
    });
    drawerLayout->addWidget(backlogButton);

    QPushButton* archiveButton = new QPushButton("Archive", m_drawer);
    archiveButton->setFlat(true);
    //archiveButton->setForegroundColor(QColor("#333"));
    QObject::connect(archiveButton, &QPushButton::clicked, this, [=](){
        this->showArchiveTab();
    });
    drawerLayout->addWidget(archiveButton);

    /*QPushButton* notesButton = new QPushButton("Notes", m_drawer);
    notesButton->setForegroundColor(QColor("#333"));
    QObject::connect(notesButton, SIGNAL(clicked()), this, SLOT(showNotesTab()));
    drawerLayout->addWidget(notesButton);*/

    /*QPushButton *focusTimerButton = new QPushButton("Focus Timer", m_drawer);
    focusTimerButton->setFlat(true);
    QObject::connect(focusTimerButton, SIGNAL(clicked()), this, SLOT(showFocusTimerDialog()));
    drawerLayout->addWidget(focusTimerButton);*/

    QPushButton* settingsButton = new QPushButton("Settings", m_drawer);
    settingsButton->setFlat(true);
    //settingsButton->setForegroundColor(QColor("#333"));
    QObject::connect(settingsButton, &QPushButton::clicked, this, [=](){
        this->showSettingsTab();
    });
    drawerLayout->addWidget(settingsButton);

    drawerLayout->addStretch(3);

    QPushButton* closeDrawerButton = new QPushButton("Close", m_drawer);
    closeDrawerButton->setFlat(true);
    //closeDrawerButton->setForegroundColor(QColor("#333"));
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
    containerLayout->setContentsMargins(0, 0, 10, 0);
        QLabel *dashboardTitleLabel = new QLabel("Dashboard", container);
        dashboardTitleLabel->setAlignment(Qt::AlignCenter | Qt::AlignCenter);
        dashboardTitleLabel->setFont(QFont("Roboto", 18, QFont::Normal));
        dashboardTitleLabel->setStyleSheet("QLabel { background-color: transparent; color: #333; }");
        containerLayout->addWidget(dashboardTitleLabel);

        QWidget *subContainer = new QWidget(container);
            QHBoxLayout *subContainerLayout = new QHBoxLayout(subContainer);
            subContainerLayout->setContentsMargins(0, 0, 0, 0);
                QWidget *chartsContainerWidget = new QWidget(subContainer);
                chartsContainerWidget->setLayout(new QHBoxLayout(chartsContainerWidget));
                chartsContainerWidget->layout()->setContentsMargins(0, 0, 0, 0);
                chartsContainerWidget->layout()->setSpacing(0);

                    BurndownChartWidget* burndownChartWidget = new BurndownChartWidget(chartsContainerWidget);
                    burndownChartWidget->setObjectName("burndownChartWidget");
                    m_widgets.insert(burndownChartWidget->objectName(), burndownChartWidget);
                    chartsContainerWidget->layout()->addWidget(burndownChartWidget);

                    TaskStatusChartWidget* taskStatusChartWidget = new TaskStatusChartWidget(chartsContainerWidget);
                    taskStatusChartWidget->setObjectName("taskStatusChartWidget");
                    m_widgets.insert(taskStatusChartWidget->objectName(), taskStatusChartWidget);
                    chartsContainerWidget->layout()->addWidget(taskStatusChartWidget);

                subContainerLayout->addWidget(chartsContainerWidget);
                subContainerLayout->setStretch(0, 4);

                QWidget *toolsContainerWidget = new QWidget(subContainer);
                QVBoxLayout *toolsContainerWidgetLayout = new QVBoxLayout(toolsContainerWidget);
                toolsContainerWidgetLayout->setContentsMargins(0, 0, 0, 0);
                toolsContainerWidget->setLayout(toolsContainerWidgetLayout);
                    QLabel *toolsTitleLabel = new QLabel("Tools", toolsContainerWidget);
                    toolsTitleLabel->setAlignment(Qt::AlignCenter | Qt::AlignCenter);
                    toolsTitleLabel->setFont(QFont("Roboto", 16, QFont::Normal));
                    toolsContainerWidget->layout()->addWidget(toolsTitleLabel);

                    /*toolsContainerWidgetLayout->addWidget(new QPushButton("Export", toolsContainerWidget));
                    toolsContainerWidgetLayout->addWidget(new QPushButton("History", toolsContainerWidget));*/

                    toolsContainerWidgetLayout->layout()->addItem(new QSpacerItem(10, 10, QSizePolicy::Expanding, QSizePolicy::Expanding));
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
    try
    {
        Router &router = Router::getInstance();

        QWidget* container = new QWidget(ui->mainWidget);
        container->setObjectName("backlogContainerWidget");
        m_widgets.insert(container->objectName(), container);
        QVBoxLayout *containerLayout = new QVBoxLayout(container);
        containerLayout->setContentsMargins(5, 5, 5, 5);
            QLabel *backlogTitleLabel = new QLabel("Backlog", container);
            backlogTitleLabel->setAlignment(Qt::AlignCenter | Qt::AlignCenter);
            backlogTitleLabel->setFont(QFont("Roboto", 18, QFont::Normal));
            backlogTitleLabel->setStyleSheet("QLabel { background-color: transparent; color: #333; }");
            containerLayout->addWidget(backlogTitleLabel);

            QWidget *actionsContainer = new QWidget(container);
            actionsContainer->setLayout(new QHBoxLayout(actionsContainer));
            actionsContainer->layout()->setContentsMargins(0, 0, 0, 10);
                QPushButton *addNewTaskButton = new QPushButton("Add New Task", actionsContainer);
                QObject::connect(addNewTaskButton, &QPushButton::clicked, this, [=](){
                    try
                    {
                        Router& router = Router::getInstance();
                        Task task = router.getRepository().createNewActiveBaseTask();
                        this->showTaskDialog(task, true, false);
                    }
                    catch(std::invalid_argument e)
                    {
                        QMessageBox(QMessageBox::Warning, "Error", e.what()).exec();
                    }
                    catch(...)
                    {
                        QMessageBox(QMessageBox::Warning, "Error", "???").exec();
                    }
                });
                actionsContainer->layout()->addWidget(addNewTaskButton);

                QPushButton *removeTaskButton = new QPushButton("Remove Task", actionsContainer);
                QObject::connect(removeTaskButton, &QPushButton::clicked, this, [=](){
                    this->showRemoveTaskDialog();
                });
                actionsContainer->layout()->addWidget(removeTaskButton);

                QPushButton *archiveByStatusButton = new QPushButton("Archive By Status", actionsContainer);
                QObject::connect(archiveByStatusButton, &QPushButton::clicked, this, [=](){
                    this->showArchiveByStatusDialog();
                });
                actionsContainer->layout()->addWidget(archiveByStatusButton);

                actionsContainer->layout()->addItem(new QSpacerItem(10, 10, QSizePolicy::Expanding, QSizePolicy::Fixed));

                m_filterInput->setParent(actionsContainer);
                m_filterInput->setPlaceholderText("Quick Search");
                m_filterInput->setFixedWidth(300);
                QObject::connect(m_filterInput, &QLineEdit::returnPressed, this, [=]() {
                    this->onApplyFilterTaskButton_Clicked();
                });
                actionsContainer->layout()->addWidget(m_filterInput);

                QPushButton *applyFilterTaskButton = new QPushButton("Apply", actionsContainer);
                QObject::connect(applyFilterTaskButton, SIGNAL(clicked()), this, SLOT(onApplyFilterTaskButton_Clicked()));
                actionsContainer->layout()->addWidget(applyFilterTaskButton);

                //actionsContainer->layout()->addWidget(new QPushButton("Sync Tasks With Trello", actionsContainer));
            containerLayout->addWidget(actionsContainer);

            QScrollArea *scrollArea = new QScrollArea(container);
            scrollArea->setWidgetResizable(true);
            scrollArea->setStyleSheet("QScrollArea { border: 0px; }");
                QWidget *scrollAreaContent = new QWidget(scrollArea);
                scrollAreaContent->setLayout(new QHBoxLayout(scrollAreaContent));
                scrollAreaContent->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
                scrollAreaContent->layout()->setContentsMargins(0, 0, 0, 15);
                    QStringList avaliableStatuses = router.getRepository().getAvaliableStatuses();
                    for(auto status : avaliableStatuses)
                    {
                        TaskListWidget* taskListWidget = new TaskListWidget(status, scrollAreaContent);
                        taskListWidget->setObjectName(status + "TaskListWidget");
                        QObject::connect(taskListWidget, SIGNAL(taskDropped(size_t, QString)), this, SLOT(onActiveTaskListWidget_TaskDropped(size_t, QString)));
                        QObject::connect(taskListWidget->list(), SIGNAL(itemDoubleClicked(QListWidgetItem*)), this, SLOT(onActiveTaskListWidget_ListWidget_ItemEntered(QListWidgetItem*)));
                        //m_widgets.insert(taskListWidget->objectName(), taskListWidget);
                        m_activeTaskListWidgets.append(taskListWidget);
                        scrollAreaContent->layout()->addWidget(taskListWidget);
                    }
                scrollArea->setWidget(scrollAreaContent);
            containerLayout->addWidget(scrollArea);
        container->setLayout(containerLayout);
        ui->mainWidget->layout()->addWidget(container);
        container->hide();
    }
    catch(...)
    {
        QMessageBox(QMessageBox::Warning, "Error", "???").exec();
    }
}

void MainWindow::setupArchiveTab()
{
    try
    {
        QWidget* container = new QWidget(ui->mainWidget);
        container->setObjectName("archiveContainerWidget");
        m_widgets.insert(container->objectName(), container);
        QVBoxLayout *containerLayout = new QVBoxLayout(container);
        containerLayout->setContentsMargins(5, 5, 5, 5);
            QLabel *archiveTitleLabel = new QLabel("Archive", container);
            archiveTitleLabel->setAlignment(Qt::AlignCenter | Qt::AlignCenter);
            archiveTitleLabel->setFont(QFont("Roboto", 18, QFont::Normal));
            archiveTitleLabel->setStyleSheet("QLabel { background-color: transparent; color: #333; }");
            containerLayout->addWidget(archiveTitleLabel);

            QWidget *actionsContainer = new QWidget(container);
            actionsContainer->setLayout(new QHBoxLayout(actionsContainer));
            actionsContainer->layout()->setContentsMargins(0, 0, 0, 10);
                QPushButton *syncButton = new QPushButton("Sync", actionsContainer);
                QObject::connect(syncButton, &QPushButton::clicked, this, [=](){
                    this->syncArchivedTasksState();
                });
                actionsContainer->layout()->addWidget(syncButton);

                QPushButton *unarchiveButton = new QPushButton("Unarchive Task", actionsContainer);
                QObject::connect(unarchiveButton, &QPushButton::clicked, this, [=](){
                    this->showUnarchiveDialog();
                });
                actionsContainer->layout()->addWidget(unarchiveButton);

                actionsContainer->layout()->addItem(new QSpacerItem(10, 10, QSizePolicy::Expanding, QSizePolicy::Fixed));
            containerLayout->addWidget(actionsContainer);

            m_archivedTaskListWidget->setParent(container);
            m_archivedTaskListWidget->setObjectName("archivedTaskListWidget");
            m_archivedTaskListWidget->setStyleSheet("QListWidget {} QListWidget::item { color: #333; padding: 10px; }");
            m_archivedTaskListWidget->setAlternatingRowColors(true);
            QObject::connect(m_archivedTaskListWidget, SIGNAL(itemDoubleClicked(QListWidgetItem*)), this, SLOT(onArcivedTaskListWidget_ItemEntered(QListWidgetItem*)));
            containerLayout->addWidget(m_archivedTaskListWidget);
        container->setLayout(containerLayout);
        ui->mainWidget->layout()->addWidget(container);
        container->hide();
    }
    catch(...)
    {
        QMessageBox(QMessageBox::Warning, "Error", "???").exec();
    }
}

void MainWindow::setupSettingsTab()
{
    try
    {
        Router &router = Router::getInstance();
        QString storeDirectory = router.getRepository().storeDirectory();
        QString calendarUrl = router.getRepository().getCalendarUrl();
        QString avaliableStatuses = router.getRepository().getAvaliableStatuses().join(";");

        QWidget* container = new QWidget(ui->mainWidget);
        container->setObjectName("settingsContainerWidget");
        m_widgets.insert(container->objectName(), container);
        QVBoxLayout *containerLayout = new QVBoxLayout(container);
        containerLayout->setContentsMargins(5, 5, 5, 5);
            QLabel *settingsTitleLabel = new QLabel("Settings", container);
            settingsTitleLabel->setAlignment(Qt::AlignCenter | Qt::AlignCenter);
            settingsTitleLabel->setFont(QFont("Roboto", 18, QFont::Normal));
            settingsTitleLabel->setStyleSheet("QLabel { background-color: transparent; color: #333; }");
            containerLayout->addWidget(settingsTitleLabel);

            QWidget *storeDirectoryWidgetContainer = new QWidget(container);
            storeDirectoryWidgetContainer->setLayout(new QHBoxLayout(storeDirectoryWidgetContainer));
            storeDirectoryWidgetContainer->layout()->setContentsMargins(0, 0, 0, 0);
                m_storeDirectoryInput->setParent(storeDirectoryWidgetContainer);
                //m_storeDirectoryInput->setLabel("Store Directory");
                m_storeDirectoryInput->setText(storeDirectory);
                m_storeDirectoryInput->setReadOnly(true);
                //m_storeDirectoryInput->setLabelFontSize(16);
                //m_storeDirectoryInput->setInkColor(QColor("#333"));
                m_storeDirectoryInput->setPlaceholderText("Here you could enter full path to root directory for TaskSpace.");
                m_storeDirectoryInput->setFont(QFont("Roboto", 16, QFont::Normal));
                m_storeDirectoryInput->setStyleSheet("QtMaterialTextField { background-color: transparent; }");
                QObject::connect(&router, SIGNAL(storeDirectoryChanged(QString)), m_storeDirectoryInput, SLOT(setText(QString)));
                storeDirectoryWidgetContainer->layout()->addWidget(m_storeDirectoryInput);

                QPushButton *selectStorageDirectoryButton = new QPushButton("Change", storeDirectoryWidgetContainer);
                QObject::connect(selectStorageDirectoryButton, SIGNAL(clicked()), this, SLOT(onSelectStorageDirectoryButton_Clicked()));
                storeDirectoryWidgetContainer->layout()->addWidget(selectStorageDirectoryButton);
            containerLayout->addWidget(storeDirectoryWidgetContainer);

            QWidget *calendarUrlConatiner = new QWidget(container);
                QHBoxLayout *calendarUrlConatinerLayout = new QHBoxLayout(calendarUrlConatiner);
                calendarUrlConatinerLayout->setContentsMargins(0, 10, 0, 0);
                    m_calendarUrlInput->setParent(calendarUrlConatiner);
                    //m_calendarUrlInput->setLabel("Calendar Url");
                    m_calendarUrlInput->setText(calendarUrl);
                    m_calendarUrlInput->setReadOnly(false);
                    //m_calendarUrlInput->setLabelFontSize(16);
                    //m_calendarUrlInput->setInkColor(QColor("#333"));
                    m_calendarUrlInput->setPlaceholderText("Here you could enter url to your calendar.");
                    m_calendarUrlInput->setFont(QFont("Roboto", 16, QFont::Normal));
                    m_calendarUrlInput->setStyleSheet("QtMaterialTextField { background-color: transparent; }");
                    QObject::connect(&router, SIGNAL(calendarUrlChanged(QString)), m_calendarUrlInput, SLOT(setText(QString)));
                    calendarUrlConatinerLayout->addWidget(m_calendarUrlInput);

                    //calendarUrlConatinerLayout->addItem(new QSpacerItem(10, 10, QSizePolicy::Expanding, QSizePolicy::Fixed));
                calendarUrlConatiner->setLayout(calendarUrlConatinerLayout);
            containerLayout->addWidget(calendarUrlConatiner);

            QWidget *avaliableStatusesListConatiner = new QWidget(container);
                QHBoxLayout *avaliableStatusesListConatinerLayout = new QHBoxLayout(avaliableStatusesListConatiner);
                avaliableStatusesListConatinerLayout->setContentsMargins(0, 10, 0, 0);
                    m_avaliableStatusesListInput->setParent(avaliableStatusesListConatiner);
                    //m_avaliableStatusesListInput->setLabel("Avaliable Statuses");
                    m_avaliableStatusesListInput->setText(avaliableStatuses);
                    m_avaliableStatusesListInput->setReadOnly(false);
                    //m_avaliableStatusesListInput->setLabelFontSize(16);
                    //m_avaliableStatusesListInput->setInkColor(QColor("#333"));
                    m_avaliableStatusesListInput->setPlaceholderText("Here you could enter some statuses for your tasks separated by \";\".");
                    m_avaliableStatusesListInput->setFont(QFont("Roboto", 16, QFont::Normal));
                    m_avaliableStatusesListInput->setStyleSheet("QtMaterialTextField { background-color: transparent; }");
                    QObject::connect(&router, SIGNAL(avaliableStatusesChanged(QString)), m_avaliableStatusesListInput, SLOT(setText(QString)));
                    avaliableStatusesListConatinerLayout->addWidget(m_avaliableStatusesListInput);

                    //calendarUrlConatinerLayout->addItem(new QSpacerItem(10, 10, QSizePolicy::Expanding, QSizePolicy::Fixed));
                avaliableStatusesListConatiner->setLayout(avaliableStatusesListConatinerLayout);
            containerLayout->addWidget(avaliableStatusesListConatiner);

            containerLayout->addItem(new QSpacerItem(10, 10, QSizePolicy::Expanding, QSizePolicy::Expanding));
            QPushButton *applySettingsButton = new QPushButton("Apply Settings (need application restart)", container);
            applySettingsButton->setFlat(true);
            QObject::connect(applySettingsButton, SIGNAL(clicked()), this, SLOT(onApplySettingsButton_Clicked()));
            containerLayout->addWidget(applySettingsButton);
        container->setLayout(containerLayout);
        ui->mainWidget->layout()->addWidget(container);
        container->hide();
    }
    catch(...)
    {
        QMessageBox(QMessageBox::Warning, "Error", "???").exec();
    }
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
    for(auto taskListWidget : m_activeTaskListWidgets)
    {
        taskListWidget->list()->clear();
    }
}

void MainWindow::showDashboardTab()
{
    m_widgets["dashboardContainerWidget"]->show();
    m_widgets["backlogContainerWidget"]->hide();
    m_widgets["archiveContainerWidget"]->hide();
    m_widgets["settingsContainerWidget"]->hide();
}

void MainWindow::showBacklogTab()
{
    m_widgets["dashboardContainerWidget"]->hide();
    m_widgets["backlogContainerWidget"]->show();
    m_widgets["archiveContainerWidget"]->hide();
    m_widgets["settingsContainerWidget"]->hide();
}

void MainWindow::showArchiveTab()
{
    m_widgets["dashboardContainerWidget"]->hide();
    m_widgets["backlogContainerWidget"]->hide();
    m_widgets["archiveContainerWidget"]->show();
    m_widgets["settingsContainerWidget"]->hide();
}

void MainWindow::showNotesTab()
{
    /*qDeleteAll(ui->mainFrame->children());
    ui->mainFrame->setLayout(new QHBoxLayout(ui->mainFrame));
    ui->mainFrame->layout()->addWidget(new QPushButton("Notes", ui->mainFrame));

    this->showGauge();*/
}

void MainWindow::showSettingsTab()
{
    m_widgets["dashboardContainerWidget"]->hide();
    m_widgets["backlogContainerWidget"]->hide();
    m_widgets["archiveContainerWidget"]->hide();
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

void MainWindow::showCalendarDialog()
{
    try
    {
        Router& router = Router::getInstance();
        QString calendarUrl = router.getRepository().getCalendarUrl();

        QDialog *calendarDialog = new QDialog(this);
        calendarDialog->setWindowTitle("Calendar");
        calendarDialog->setMinimumSize(800, 600);
            QVBoxLayout* calendarDialogLayout = new QVBoxLayout(calendarDialog);
            calendarDialogLayout->setContentsMargins(0, 0, 0, 0);
                QWidget* container = new QWidget(ui->mainWidget);
                    QVBoxLayout *containerLayout = new QVBoxLayout(container);
                    containerLayout->setContentsMargins(0, 0, 0, 0);
                        QWebEngineView *view = new QWebEngineView(container);
                        view->load(QUrl(calendarUrl));
                        view->show();
                        containerLayout->addWidget(view);
                    container->setLayout(containerLayout);
                calendarDialogLayout->addWidget(container);
            calendarDialog->setLayout(calendarDialogLayout);
        //calendarDialog->showMaximized();
        calendarDialog->showNormal();
    }
    catch(...)
    {
        QMessageBox(QMessageBox::Warning, "Error", "???").exec();
    }
}

void MainWindow::showTaskDialog(Task task, bool newTask, bool archived)
{
    QDialog *taskDialog = new QDialog(this);
    taskDialog->setWindowTitle(task.decoratedBaseInformation());
    taskDialog->setMinimumSize(800, 600);
    taskDialog->setBaseSize(800, 600);
    taskDialog->setStyleSheet("QDialog {background-color: #fff;}");
    QVBoxLayout* dialogLayout = new QVBoxLayout(taskDialog);
    dialogLayout->setContentsMargins(0, 0, 0, 0);
       QWidget* containerWidget = new QWidget(taskDialog);
           QVBoxLayout* containerWidgetLayout = new QVBoxLayout(containerWidget);
                TaskViewerWidget* taskViewerWidget = new TaskViewerWidget(containerWidget);
                taskViewerWidget->setEditingEnable(newTask);
                taskViewerWidget->setTaskIndex(long(task.index()));
                taskViewerWidget->setTaskTitle(task.title());
                taskViewerWidget->setTaskDescription(task.description());
                taskViewerWidget->setTaskDueToDate(task.dueToDate());
                taskViewerWidget->setTaskDueToDateEnabled(task.dueToDateEnabled());
                taskViewerWidget->setTaskEstimatedTime(task.estimatedTime());
                taskViewerWidget->setTaskActualTime(task.actualTime());
                QObject::connect(taskViewerWidget,
                                 SIGNAL(taskUpdated(size_t, QString, QString, QDate, bool, QTime, QTime)),
                                 this,
                                 SLOT(onTaskViewerWidget_TaskUpdated(size_t, QString, QString, QDate, bool, QTime, QTime)));
                containerWidgetLayout->addWidget(taskViewerWidget);

                QWidget* actionsContainerWidget = new QWidget(containerWidget);
                   QHBoxLayout* actionsContainerWidgetLayout = new QHBoxLayout(actionsContainerWidget);

                       if(!archived)
                       {
                           QPushButton *editOrLockButton = new QPushButton(actionsContainerWidget);
                           editOrLockButton->setFlat(true);
                           editOrLockButton->setText("Edit/Lock");
                           QObject::connect(editOrLockButton, SIGNAL(clicked()), taskViewerWidget, SLOT(changeEditingEnableStatus()));
                           actionsContainerWidgetLayout->addWidget(editOrLockButton);

                           actionsContainerWidgetLayout->addItem(new QSpacerItem(10, 10, QSizePolicy::Expanding, QSizePolicy::Fixed));

                           QPushButton *saveTaskButton = new QPushButton("Save", actionsContainerWidget);
                           saveTaskButton->setFlat(true);
                           saveTaskButton->setDefault(true);
                           QObject::connect(saveTaskButton, SIGNAL(clicked()), taskViewerWidget, SLOT(saveTaskData()));
                           actionsContainerWidgetLayout->addWidget(saveTaskButton);

                       }
                       else
                       {
                           actionsContainerWidgetLayout->addItem(new QSpacerItem(10, 10, QSizePolicy::Expanding, QSizePolicy::Fixed));
                       }

                       QPushButton *closeButton = new QPushButton("Close", actionsContainerWidget);
                       closeButton->setFlat(true);
                       QObject::connect(closeButton, SIGNAL(clicked()), taskDialog, SLOT(close()));
                       actionsContainerWidgetLayout->addWidget(closeButton);
                   actionsContainerWidget->setLayout(actionsContainerWidgetLayout);
                containerWidgetLayout->addWidget(actionsContainerWidget);
            containerWidget->setLayout(containerWidgetLayout);
        dialogLayout->addWidget(containerWidget);
    taskDialog->setLayout(dialogLayout);
    taskDialog->exec();
}

void MainWindow::onSelectStorageDirectoryButton_Clicked()
{
    QString path = QFileDialog::getExistingDirectory(this, "Select storage directory", "");
    if(path.length() > 0)
    {
        m_storeDirectoryInput->setText(path);
    }
}

void MainWindow::onRouter_TasksUpdated()
{
    try
    {
        //qDebug() << "MainWindow::onRouter_TasksUpdated";

        // update backlog lists
        Router& router = Router::getInstance();
        for(auto taskListWidget : m_activeTaskListWidgets)
        {
            taskListWidget->list()->clear();
            QList<Task> tasks = router.getRepository().getTasksByStatus(taskListWidget->status(), m_filterInput->text());
            for(auto task : tasks)
            {
                QListWidgetItem *item = new QListWidgetItem();
                item->setText(task.decoratedBaseInformation());
                taskListWidget->list()->addItem(item);
            }
        }

        // update dashboard charts
        QWidget* rawTaskStatusBarWidget = m_widgets["taskStatusChartWidget"];
        TaskStatusChartWidget* taskStatusChartWidget = qobject_cast<TaskStatusChartWidget*>(rawTaskStatusBarWidget);
        if(taskStatusChartWidget != nullptr)
        {
            QStringList avaliableStatuses = router.getRepository().getAvaliableStatuses();
            QList<int> taskCountByStatus;
            for(auto status : avaliableStatuses)
            {
                taskCountByStatus << router.getRepository().getTaskCountByStatus(status);
            }
            taskStatusChartWidget->updateChartWidget(taskCountByStatus, avaliableStatuses);
        }
    }
    catch(...)
    {
        QMessageBox(QMessageBox::Warning, "Error", "???").exec();
    }
}

void MainWindow::showRemoveTaskDialog()
{
    QDialog *removeTaskDialog = new QDialog(this);
    removeTaskDialog->setWindowTitle("Remove Task?");
    removeTaskDialog->setMinimumSize(300, 150);
    removeTaskDialog->setBaseSize(300, 150);
    removeTaskDialog->setStyleSheet("QDialog {background-color: #fff;}");
    QVBoxLayout* dialogLayout = new QVBoxLayout(removeTaskDialog);
    dialogLayout->setContentsMargins(15, 15, 15, 0);
       QWidget* containerWidget = new QWidget(removeTaskDialog);
           QVBoxLayout* containerWidgetLayout = new QVBoxLayout(containerWidget);
           containerWidgetLayout->setContentsMargins(0, 0, 0, 0);

                TaskIndexInputWidget* removeTaskIndexInput = new TaskIndexInputWidget(containerWidget);
                QObject::connect(removeTaskIndexInput, SIGNAL(indexSelected(size_t)), this, SLOT(onRemoveTaskInputWidget_IndexSelected(size_t)));
                containerWidgetLayout->addWidget(removeTaskIndexInput);

                QWidget* actionsContainerWidget = new QWidget(containerWidget);
                   QHBoxLayout* actionsContainerWidgetLayout = new QHBoxLayout(actionsContainerWidget);
                   actionsContainerWidgetLayout->setContentsMargins(0, 0, 0, 0);
                       actionsContainerWidgetLayout->addItem(new QSpacerItem(10, 10, QSizePolicy::Expanding, QSizePolicy::Fixed));

                       QPushButton *removeButton = new QPushButton("Remove", actionsContainerWidget);
                       QObject::connect(removeButton, &QPushButton::clicked, removeTaskDialog, [=](){
                            removeTaskIndexInput->getResult();
                            removeTaskDialog->close();
                       });
                       actionsContainerWidgetLayout->addWidget(removeButton);

                       QPushButton *closeButton = new QPushButton("Close", actionsContainerWidget);
                       QObject::connect(closeButton, SIGNAL(clicked()), removeTaskDialog, SLOT(close()));
                       actionsContainerWidgetLayout->addWidget(closeButton);
                   actionsContainerWidget->setLayout(actionsContainerWidgetLayout);
                containerWidgetLayout->addWidget(actionsContainerWidget);
            containerWidget->setLayout(containerWidgetLayout);
        dialogLayout->addWidget(containerWidget);
    removeTaskDialog->setLayout(dialogLayout);
    removeTaskDialog->exec();
}

void MainWindow::showArchiveByStatusDialog()
{
    try
    {
        Router &router = Router::getInstance();

        QDialog *dialog = new QDialog(this);
        dialog->setWindowTitle("Archive Tasks?");
        dialog->setMinimumSize(300, 100);
        dialog->setBaseSize(300, 100);
        dialog->setStyleSheet("QDialog {background-color: #fff;}");
        QVBoxLayout* dialogLayout = new QVBoxLayout(dialog);
        dialogLayout->setContentsMargins(15, 15, 15, 0);
           QWidget* containerWidget = new QWidget(dialog);
               QVBoxLayout* containerWidgetLayout = new QVBoxLayout(containerWidget);
               containerWidgetLayout->setContentsMargins(0, 0, 0, 0);

                   QComboBox *statusInput = new QComboBox(containerWidget);
                   statusInput->setFont(QFont("Roboto", 16, QFont::Normal));
                   statusInput->addItems(router.getRepository().getAvaliableStatuses());
                   containerWidgetLayout->addWidget(statusInput);

                    QWidget* actionsContainerWidget = new QWidget(containerWidget);
                       QHBoxLayout* actionsContainerWidgetLayout = new QHBoxLayout(actionsContainerWidget);
                       actionsContainerWidgetLayout->setContentsMargins(0, 0, 0, 0);
                           actionsContainerWidgetLayout->addItem(new QSpacerItem(10, 10, QSizePolicy::Expanding, QSizePolicy::Fixed));

                           QPushButton *archiveButton = new QPushButton("Archive", actionsContainerWidget);
                           QObject::connect(archiveButton, &QPushButton::clicked, dialog, [=](){
                               Router &router = Router::getInstance();
                               QString status = statusInput->currentText();
                               router.getRepository().archiveTasksByStatus(status);
                               dialog->close();
                           });
                           actionsContainerWidgetLayout->addWidget(archiveButton);

                           QPushButton *closeButton = new QPushButton("Close", actionsContainerWidget);
                           QObject::connect(closeButton, SIGNAL(clicked()), dialog, SLOT(close()));
                           actionsContainerWidgetLayout->addWidget(closeButton);
                       actionsContainerWidget->setLayout(actionsContainerWidgetLayout);
                    containerWidgetLayout->addWidget(actionsContainerWidget);
                containerWidget->setLayout(containerWidgetLayout);
            dialogLayout->addWidget(containerWidget);
        dialog->setLayout(dialogLayout);
        dialog->exec();
    }
    catch(...)
    {
        QMessageBox(QMessageBox::Warning, "Error", "???").exec();
    }
}

void MainWindow::onActiveTaskListWidget_ListWidget_ItemEntered(QListWidgetItem *taskListWidgetItem)
{
    try
    {
        Router& router = Router::getInstance();
        QString itemText = taskListWidgetItem->text();

        QRegExp rawIndexRegExp = QRegExp("\\[(\\s)*[0-9]*(\\s)*\\]");
        rawIndexRegExp.indexIn(itemText);
        QStringList rawIndexRegExpResult = rawIndexRegExp.capturedTexts();
        if(rawIndexRegExpResult.length() <= 0)
        {
            QMessageBox(QMessageBox::Warning, "Parse Error", "Can not find taskIndex " + itemText).exec();
            return;
        }
        QString rawTaskIndex = rawIndexRegExpResult.first();
        rawTaskIndex = rawTaskIndex.remove("[");
        rawTaskIndex = rawTaskIndex.remove("]");

        bool ok = false;
        size_t taskIndex = rawTaskIndex.toUInt(&ok);
        if(!ok) {
            QMessageBox(QMessageBox::Warning, "Parse Error", "Can not parse taskIndex in " + rawTaskIndex).exec();
            return;
        }

        Task task = router.getRepository().getActiveTaskByIndex(taskIndex);
        this->showTaskDialog(task, false, false);
    }
    catch(std::invalid_argument e)
    {
        QMessageBox(QMessageBox::Warning, "Error", e.what()).exec();
    }
    catch(...)
    {
        QMessageBox(QMessageBox::Warning, "Error", "???").exec();
    }
}

void MainWindow::onArcivedTaskListWidget_ItemEntered(QListWidgetItem *taskListWidgetItem)
{
    try
    {
        Router& router = Router::getInstance();
        QString itemText = taskListWidgetItem->text();

        QRegExp rawIndexRegExp = QRegExp("\\[(\\s)*[0-9]*(\\s)*\\]");
        rawIndexRegExp.indexIn(itemText);
        QStringList rawIndexRegExpResult = rawIndexRegExp.capturedTexts();
        if(rawIndexRegExpResult.length() <= 0)
        {
            QMessageBox(QMessageBox::Warning, "Parse Error", "Can not find taskIndex " + itemText).exec();
            return;
        }
        QString rawTaskIndex = rawIndexRegExpResult.first();
        rawTaskIndex = rawTaskIndex.remove("[");
        rawTaskIndex = rawTaskIndex.remove("]");

        bool ok = false;
        size_t taskIndex = rawTaskIndex.toUInt(&ok);
        if(!ok) {
            QMessageBox(QMessageBox::Warning, "Parse Error", "Can not parse taskIndex in " + rawTaskIndex).exec();
            return;
        }

        Task task = router.getRepository().getArchivedTaskByIndex(taskIndex);
        this->showTaskDialog(task, false, true);
    }
    catch(std::invalid_argument e)
    {
        QMessageBox(QMessageBox::Warning, "Error", e.what()).exec();
    }
    catch(...)
    {
        QMessageBox(QMessageBox::Warning, "Error", "???").exec();
    }
}

void MainWindow::onActiveTaskListWidget_TaskDropped(size_t taskIndex, QString status)
{
    try
    {
        Router& router = Router::getInstance();
        router.getRepository().updateTaskStatus(taskIndex, status);
    }
    catch(std::invalid_argument e)
    {
        QMessageBox(QMessageBox::Warning, "Error", e.what()).exec();
    }
    catch(...)
    {
        QMessageBox(QMessageBox::Warning, "Error", "???").exec();
    }
}

void MainWindow::onTaskViewerWidget_TaskUpdated(size_t index, QString title, QString description, QDate dueToDate, bool dueToDateEnabled, QTime estimatedTime, QTime actualTime)
{
    try
    {
        Router& router = Router::getInstance();
        router.getRepository().updateTaskInfo(index, title, description, dueToDate, dueToDateEnabled, estimatedTime, actualTime);
    }
    catch(std::invalid_argument e)
    {
        QMessageBox(QMessageBox::Warning, "Error", e.what()).exec();
    }
    catch(...)
    {
        QMessageBox(QMessageBox::Warning, "Error", "???").exec();
    }
}

void MainWindow::onRemoveTaskInputWidget_IndexSelected(size_t index)
{
    try
    {
        Router& router = Router::getInstance();
        router.getRepository().removeTask(index);
    }
    catch(std::invalid_argument e)
    {
        QMessageBox(QMessageBox::Warning, "Error", e.what()).exec();
    }
    catch(...)
    {
        QMessageBox(QMessageBox::Warning, "Error", "???").exec();
    }
}

void MainWindow::onApplyFilterTaskButton_Clicked()
{
    this->onRouter_TasksUpdated();
}

void MainWindow::onApplySettingsButton_Clicked()
{
    try
    {
        Router &router = router.getInstance();

        QString storeDirectory = m_storeDirectoryInput->text();
        if(!storeDirectory.isEmpty())
        {
            router.getRepository().setStoreDirectory(storeDirectory);
        }

        QString calendarUrl = m_calendarUrlInput->text();
        router.getRepository().setCalendarUrl(calendarUrl);

        QString avaliableStatuses = m_avaliableStatusesListInput->text();
        router.getRepository().setAvaliableStatuses(avaliableStatuses.split(";"));
    }
    catch(...)
    {
        QMessageBox(QMessageBox::Warning, "Error", "???").exec();
    }
}

void MainWindow::showUnarchiveDialog()
{
    try
    {
        Router &router = router.getInstance();

        QDialog *dialog = new QDialog(this);
        dialog->setWindowTitle("Unarchive Task?");
        dialog->setMinimumSize(300, 150);
        dialog->setBaseSize(300, 150);
        dialog->setStyleSheet("QDialog {background-color: #fff;}");
        QVBoxLayout* dialogLayout = new QVBoxLayout(dialog);
        dialogLayout->setContentsMargins(15, 15, 15, 0);
           QWidget* containerWidget = new QWidget(dialog);
               QVBoxLayout* containerWidgetLayout = new QVBoxLayout(containerWidget);
               containerWidgetLayout->setContentsMargins(0, 0, 0, 0);

                   QtMaterialTextField *indexInput = new QtMaterialTextField(containerWidget);
                   indexInput->setText("");
                   indexInput->setLabel("Index:");
                   indexInput->setLabelFontSize(16);
                   indexInput->setInkColor(QColor("#333"));
                   indexInput->setPlaceholderText("Enter the task [index], please.");
                   indexInput->setFont(QFont("Roboto", 16, QFont::Normal));
                   containerWidgetLayout->addWidget(indexInput);

                   QComboBox *statusInput = new QComboBox(containerWidget);
                   statusInput->setFont(QFont("Roboto", 16, QFont::Normal));
                   statusInput->addItems(router.getRepository().getAvaliableStatuses());
                   containerWidgetLayout->addWidget(statusInput);

                   QWidget* actionsContainerWidget = new QWidget(containerWidget);
                      QHBoxLayout* actionsContainerWidgetLayout = new QHBoxLayout(actionsContainerWidget);
                      actionsContainerWidgetLayout->setContentsMargins(0, 0, 0, 0);
                          actionsContainerWidgetLayout->addItem(new QSpacerItem(10, 10, QSizePolicy::Expanding, QSizePolicy::Fixed));

                          QPushButton *unarchiveButton = new QPushButton("Unarchive", actionsContainerWidget);
                          QObject::connect(unarchiveButton, &QPushButton::clicked, dialog, [=](){
                              bool ok = false;
                              size_t index = indexInput->text().toUInt(&ok);
                              if(!ok) return;

                              QString status = statusInput->currentText();

                              try
                              {
                                  Router &router = Router::getInstance();
                                  Task task = router.getRepository().getArchivedTaskByIndex(index);
                                  router.getRepository().unarchiveTask(task, status);
                                  dialog->close();
                                  this->syncArchivedTasksState();
                              }
                              catch(...)
                              {
                                  QMessageBox(QMessageBox::Warning, "Error", "???").exec();
                              }
                          });
                          actionsContainerWidgetLayout->addWidget(unarchiveButton);

                          QPushButton *closeButton = new QPushButton("Close", actionsContainerWidget);
                          QObject::connect(closeButton, SIGNAL(clicked()), dialog, SLOT(close()));
                          actionsContainerWidgetLayout->addWidget(closeButton);
                      actionsContainerWidget->setLayout(actionsContainerWidgetLayout);
                   containerWidgetLayout->addWidget(actionsContainerWidget);
                containerWidget->setLayout(containerWidgetLayout);
            dialogLayout->addWidget(containerWidget);
        dialog->setLayout(dialogLayout);
        dialog->exec();
    }
    catch(...)
    {
        QMessageBox(QMessageBox::Warning, "Error", "???").exec();
    }
}

void MainWindow::syncArchivedTasksState()
{
    try
    {
        Router& router = Router::getInstance();
        QList<Task> archivedTasks = router.getRepository().getArchivedTasks();
        m_archivedTaskListWidget->clear();
        for(auto task : archivedTasks)
        {
            m_archivedTaskListWidget->addItem(task.decoratedBaseInformation());
        }
    }
    catch(...)
    {
        QMessageBox(QMessageBox::Warning, "Error", "???").exec();
    }
}

