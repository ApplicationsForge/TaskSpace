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
    // установка оформления statusBar
    ui->statusBar->setStyleSheet("background-color: #333; color: #33bb33");
    ui->statusBar->setFont(QFont("Consolas", 14));
    ui->statusBar->showMessage(tr("State: ready 0123456789"));

    // установка оформления меню слева
    QVBoxLayout *drawerLayout = new QVBoxLayout(m_drawer);
    m_drawer->setDrawerLayout(drawerLayout);

    QtMaterialFlatButton* dashboardButton = new QtMaterialFlatButton("Dashboard", m_drawer);
    drawerLayout->addWidget(dashboardButton);

    QtMaterialFlatButton* backlogButton = new QtMaterialFlatButton("Backlog", m_drawer);
    drawerLayout->addWidget(backlogButton);

    QtMaterialFlatButton* calendarButton = new QtMaterialFlatButton("Calendar", m_drawer);
    drawerLayout->addWidget(calendarButton);

    QtMaterialFlatButton* notesButton = new QtMaterialFlatButton("Notes", m_drawer);
    drawerLayout->addWidget(notesButton);


    /*QVector<QString> labels = {"Motion", "Style", "Layout", "Components", "Patterns", "Growth & communications", "Usability", "Platforms", "Resources"};
    QVector<QString>::iterator it;
    for (it = labels.begin(); it != labels.end(); ++it) {
        QLabel *label = new QLabel(*it);
        label->setMinimumHeight(30);
        label->setFont(QFont("Roboto", 10, QFont::Medium));
        drawerLayout->addWidget(label);
    }*/

    drawerLayout->addStretch(3);
    m_drawer->setContentsMargins(10, 0, 0, 0);
    m_drawer->setClickOutsideToClose(true);
    m_drawer->setOverlayMode(true);

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
    ui->mainToolBar->setStyleSheet("QToolBar { border: 0px }");

    ui->backlogScrollArea->setLayout(new QHBoxLayout(ui->backlogScrollArea));
    ui->backlogScrollArea->layout()->addWidget(new QtMaterialRaisedButton("Hello World", ui->backlogScrollArea));
}
