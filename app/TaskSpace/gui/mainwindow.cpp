#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_appBar(new QtMaterialAppBar(this)),
    m_drawer(new QtMaterialDrawer(this))
{
    ui->setupUi(this);

    this->setupWidgets();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setupWidgets()
{
    // устновка темной темы
    this->setStyleSheet("background-color: #555;");

    // установка оформления statusBar
    ui->statusBar->setStyleSheet("background-color: #333; color: #33bb33");
    ui->statusBar->setFont(QFont("Consolas", 14));
    ui->statusBar->showMessage(tr("State: ready 0123456789"));

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
    menuTitleLabel->setStyleSheet("QLabel { background-color: transparent; color: #fff; }");
    drawerLayout->addWidget(menuTitleLabel);

    QtMaterialFlatButton* dashboardButton = new QtMaterialFlatButton("Dashboard", m_drawer);
    dashboardButton->setForegroundColor(QColor("#fff"));
    QObject::connect(dashboardButton, SIGNAL(clicked()), this, SLOT(showDashboardTab()));
    drawerLayout->addWidget(dashboardButton);

    QtMaterialFlatButton* backlogButton = new QtMaterialFlatButton("Backlog", m_drawer);
    backlogButton->setForegroundColor(QColor("#fff"));
    QObject::connect(backlogButton, SIGNAL(clicked()), this, SLOT(showBacklogTab()));
    drawerLayout->addWidget(backlogButton);

    QtMaterialFlatButton* calendarButton = new QtMaterialFlatButton("Calendar", m_drawer);
    calendarButton->setForegroundColor(QColor("#fff"));
    QObject::connect(calendarButton, SIGNAL(clicked()), this, SLOT(showCalendarTab()));
    drawerLayout->addWidget(calendarButton);

    QtMaterialFlatButton* notesButton = new QtMaterialFlatButton("Notes", m_drawer);
    notesButton->setForegroundColor(QColor("#fff"));
    QObject::connect(notesButton, SIGNAL(clicked()), this, SLOT(showNotesTab()));
    drawerLayout->addWidget(notesButton);

    drawerLayout->addStretch(3);

    QtMaterialFlatButton* closeDrawerButton = new QtMaterialFlatButton("Close", m_drawer);
    closeDrawerButton->setForegroundColor(QColor("#fff"));
    QObject::connect(closeDrawerButton, SIGNAL(clicked()), m_drawer, SLOT(closeDrawer()));
    drawerLayout->addWidget(closeDrawerButton);

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

    this->showBacklogTab();
}

void MainWindow::showDashboardTab()
{

    qDeleteAll(ui->mainFrame->children());
    QHBoxLayout *layout = new QHBoxLayout(ui->mainFrame);

    QFrame *todotasksFrame = new QFrame(ui->mainFrame);
    todotasksFrame->setLayout(new QVBoxLayout(todotasksFrame));
        QLabel *todotasksLabel = new QLabel("ToDo Tasks", todotasksFrame);
        todotasksLabel->setAlignment(Qt::AlignCenter | Qt::AlignCenter);
        todotasksLabel->setFont(QFont("Roboto", 16, QFont::Normal));
        todotasksFrame->layout()->addWidget(todotasksLabel);
        todotasksFrame->layout()->addWidget(new QtMaterialRaisedButton("Dashboard", todotasksFrame));
        todotasksFrame->layout()->addWidget(new QtMaterialRaisedButton("Dashboard", todotasksFrame));
        todotasksFrame->layout()->addWidget(new QtMaterialRaisedButton("Dashboard", todotasksFrame));
        todotasksFrame->layout()->addItem(new QSpacerItem(10, 10, QSizePolicy::Expanding, QSizePolicy::Expanding));
    layout->addWidget(todotasksFrame);
    layout->setStretch(0, 4);

    QFrame *toolsFrame = new QFrame(ui->mainFrame);
    QVBoxLayout *toolsFrameLayout = new QVBoxLayout(toolsFrame);
    toolsFrame->setLayout(toolsFrameLayout);
        QtMaterialRaisedButton *focusTimerButton = new QtMaterialRaisedButton("Focus Timer", toolsFrame);
        QObject::connect(focusTimerButton, SIGNAL(clicked()), this, SLOT(showFocusTimerDialog()));
        toolsFrameLayout->addWidget(focusTimerButton);

        toolsFrameLayout->layout()->addItem(new QSpacerItem(10, 10, QSizePolicy::Expanding, QSizePolicy::Expanding));
    layout->addWidget(toolsFrame);
    layout->setStretch(1, 1);

    ui->mainFrame->setLayout(layout);
}

void MainWindow::showBacklogTab()
{
    qDeleteAll(ui->mainFrame->children());
    QVBoxLayout *layout = new QVBoxLayout(ui->mainFrame);
        QScrollArea *scrollArea = new QScrollArea(ui->mainFrame);
        scrollArea->setWidgetResizable(true);
        scrollArea->setStyleSheet("QScrollArea { border: 0px; }");

            QWidget *scrollAreaContent = new QWidget(scrollArea);
            scrollAreaContent->setLayout(new QHBoxLayout(scrollAreaContent));
            scrollAreaContent->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
                TaskListWidget* productBacklogListWidget = new TaskListWidget("Product Backlog", scrollAreaContent);
                productBacklogListWidget->list()->setDragEnabled(true);
                productBacklogListWidget->list()->setDropIndicatorShown(true);
                productBacklogListWidget->list()->setDragDropMode(QAbstractItemView::DragDrop);
                    for(int i = 0; i < 100; i++)
                    {
                        QListWidgetItem *item = new QListWidgetItem(QString::number(i));
                        /*MyListWidgetItem* board =
                                new MyListWidgetItem(QString::number(i), "test task", "","", QStringList(), QStringList(), productBacklogListWidget);
                        item->setSizeHint(board->minimumSizeHint());*/
                        productBacklogListWidget->list()->addItem(item);
                        //productBacklogListWidget->list()->setItemWidget(item, board);
                    }
                scrollAreaContent->layout()->addWidget(productBacklogListWidget);

                TaskListWidget* sprintBacklogListWidget = new TaskListWidget("Sprint Backlog", scrollAreaContent);
                sprintBacklogListWidget->list()->setDragEnabled(true);
                sprintBacklogListWidget->list()->setDropIndicatorShown(true);
                sprintBacklogListWidget->list()->setDragDropMode(QAbstractItemView::DragDrop);
                scrollAreaContent->layout()->addWidget(sprintBacklogListWidget);

                TaskListWidget* inProgressListWidget = new TaskListWidget("In Progress", scrollAreaContent);
                inProgressListWidget->list()->setDragEnabled(true);
                inProgressListWidget->list()->setDropIndicatorShown(true);
                inProgressListWidget->list()->setDragDropMode(QAbstractItemView::DragDrop);
                scrollAreaContent->layout()->addWidget(inProgressListWidget);

                TaskListWidget* testingListWidget = new TaskListWidget("Testing", scrollAreaContent);
                testingListWidget->list()->setDragEnabled(true);
                testingListWidget->list()->setDropIndicatorShown(true);
                testingListWidget->list()->setDragDropMode(QAbstractItemView::DragDrop);
                scrollAreaContent->layout()->addWidget(testingListWidget);

                TaskListWidget* doneListWidget = new TaskListWidget("Done", scrollAreaContent);
                doneListWidget->list()->setDragEnabled(true);
                doneListWidget->list()->setDropIndicatorShown(true);
                doneListWidget->list()->setDragDropMode(QAbstractItemView::DragDrop);
                scrollAreaContent->layout()->addWidget(doneListWidget);
        scrollArea->setWidget(scrollAreaContent);
    layout->addWidget(scrollArea);
    ui->mainFrame->setLayout(layout);
}

void MainWindow::showCalendarTab()
{
    qDeleteAll(ui->mainFrame->children());
    ui->mainFrame->setLayout(new QHBoxLayout(ui->mainFrame));
    ui->mainFrame->layout()->addWidget(new QtMaterialRaisedButton("Calendar", ui->mainFrame));
}

void MainWindow::showNotesTab()
{
    qDeleteAll(ui->mainFrame->children());
    ui->mainFrame->setLayout(new QHBoxLayout(ui->mainFrame));
    ui->mainFrame->layout()->addWidget(new QtMaterialRaisedButton("Notes", ui->mainFrame));

    this->showGauge();
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

    ui->mainFrame->layout()->addWidget(speedGauge);
}

void MainWindow::showFocusTimerDialog()
{
    QDialog *focusTimerDialog = new QDialog(this);
    focusTimerDialog->setWindowTitle("Focus Timer Dialog");
    focusTimerDialog->setMinimumSize(400, 300);
    focusTimerDialog->show();
}
