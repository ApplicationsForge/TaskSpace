#include "repository.h"

Repository::Repository(QObject *parent) :
    QObject(parent),
    m_settingsManager(new SettingsManager()),
    m_dbPath("")
{
    this->loadSettings();

    if(!Repository::resolveDb(m_dbPath))
    {
        QMessageBox(QMessageBox::Critical, "Error", "Can not find and create Database").exec();
        qApp->exit(0);
    }

    this->loadStatuses();
    this->loadTeammates();
    this->loadTasks();
}

Repository::~Repository()
{
}

QString Repository::helloString() const
{
    return m_helloString;
}

void Repository::setHelloString(const QString &helloString)
{
    m_helloString = helloString;
}

QString Repository::dbPath() const
{
    return m_dbPath;
}

void Repository::setDbPath(const QString &dbPath)
{
    m_dbPath = dbPath;
    emit this->dbPathChanged(m_dbPath);
}

QList<Task> Repository::tasks() const
{
    return m_tasks;
}

QList<Task> Repository::tasksByStatus(size_t statusId)
{
    QList<Task> result = QList<Task>();

    for(auto task : m_tasks)
    {
        if(task.status().id() == statusId)
        {
            result.append(task);
        }
    }

    return result;
}

void Repository::setTasks(const QList<Task> &tasks)
{
    m_tasks = tasks;
}

QList<Status> Repository::statuses() const
{
    return m_statuses;
}

void Repository::setStatuses(const QList<Status> &statuses)
{
    m_statuses = statuses;
}

QList<Teammate> Repository::teammates() const
{
    return m_teammates;
}

void Repository::setTeammates(const QList<Teammate> &teammates)
{
    m_teammates = teammates;
}

void Repository::loadSettings()
{
    try {
        this->setDbPath(m_settingsManager->get("Main", "DBPath").toString());
    }
    catch(std::invalid_argument e) {
        QMessageBox(QMessageBox::Warning, "Error", e.what()).exec();
        this->setDbPath("");
    }
}

void Repository::loadStatuses()
{
    m_statuses.clear();

    try
    {
        QStringList sql;
        sql << "SELECT * FROM statuses;";

        QList<QSqlRecord> response = SQLiteAdapter::executeSQL(m_dbPath, sql.join(" "));
        for(auto item : response) {
            size_t id = size_t(item.value("id").toUInt());
            QString name = item.value("name").toString();

            Status status(id, name);
            m_statuses.append(status);
        }
    }
    catch(std::invalid_argument e)
    {
        QMessageBox(QMessageBox::Warning, "Error", e.what()).exec();
    }
    catch(std::runtime_error e)
    {
        QMessageBox(QMessageBox::Warning, "Error", e.what()).exec();
    }
}

void Repository::loadTeammates()
{
    m_teammates.clear();
    m_teammates.append(Teammate(0));

    try
    {
        QStringList sql;
        sql << "SELECT * FROM teammates;";

        QList<QSqlRecord> response = SQLiteAdapter::executeSQL(m_dbPath, sql.join(" "));
        for(auto item : response) {
            size_t id = size_t(item.value("id").toUInt());
            QString name = item.value("name").toString();
            Teammate teammate(id, name);
            m_teammates.append(teammate);
        }
    }
    catch(std::invalid_argument e)
    {
        QMessageBox(QMessageBox::Warning, "Error", e.what()).exec();
    }
    catch(std::runtime_error e)
    {
        QMessageBox(QMessageBox::Warning, "Error", e.what()).exec();
    }
}

void Repository::loadTasks()
{
    m_tasks.clear();

    try
    {
        QStringList selectAllTasks;
        selectAllTasks << "SELECT * FROM tasks;";

        QList<QSqlRecord> response = SQLiteAdapter::executeSQL(m_dbPath, selectAllTasks.join(" "));
        for(auto item : response) {
            size_t id = size_t(item.value("id").toUInt());
            QString title = item.value("title").toString();
            size_t status_id = size_t(item.value("status_id").toUInt());
            QDate dueTo = item.value("due_to").toDate();
            QString description = item.value("description").toString();
            QString rawTags = item.value("tags").toString();
            size_t teammate_id = size_t(item.value("teammate_id").toUInt());

            Status status = this->findStatus(status_id);
            Teammate teammate = this->findTeammate(teammate_id);

            Task task(id, title, status, dueTo, description, rawTags.split(","), teammate);
            m_tasks.append(task);
        }
    }
    catch(std::invalid_argument e)
    {
        QMessageBox(QMessageBox::Warning, "Error", e.what()).exec();
    }
    catch(std::runtime_error e)
    {
        QMessageBox(QMessageBox::Warning, "Error", e.what()).exec();
    }
}

Teammate Repository::findTeammate(size_t id)
{
    for(auto teammate : m_teammates)
    {
        if(teammate.id() == id)
        {
            return teammate;
        }
    }

    throw std::invalid_argument("Teammate with id = " + std::to_string(id) + " does not exist");
}

Status Repository::findStatus(size_t id)
{
    for(auto status : m_statuses)
    {
        if(status.id() == id)
        {
            return status;
        }
    }

    throw std::invalid_argument("Status with id = " + std::to_string(id) + " does not exist");
}

bool Repository::resolveDb(QString path)
{
    if(QFile::exists(path))
    {
       return true;
    }

    return Repository::initDb(path);
}

bool Repository::initDb(QString path)
{
    QStringList createTeammatesTableRequest;
    createTeammatesTableRequest
        << "CREATE TABLE `teammates` ("
        << "`id`	INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT UNIQUE,"
        << "`nickname`	TEXT NOT NULL UNIQUE,"
        << "`name`	TEXT,"
        << "`email`	TEXT"
        << ");";
    qDebug() << SQLiteAdapter::executeSQL(path, createTeammatesTableRequest.join(' '));

    QStringList createStatusesTableRequest;
    createStatusesTableRequest
       << "CREATE TABLE `statuses` ("
       << "`id`	INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT UNIQUE,"
       << "`name`	INTEGER NOT NULL UNIQUE"
       << ");";
    qDebug() << SQLiteAdapter::executeSQL(path, createStatusesTableRequest.join(' '));

    QStringList createTaskTableRequest;
    createTaskTableRequest
        << "CREATE TABLE `tasks` ("
        << "`id`	INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT UNIQUE,"
        << "`title`	INTEGER NOT NULL,"
        << "`status_id`	INTEGER NOT NULL,"
        << "`due_to`	TEXT,"
        << "`description`	TEXT,"
        << "`tags`	TEXT,"
        << "`teammate_id`	INTEGER,"
        << "FOREIGN KEY (teammate_id) REFERENCES teammates (id),"
        << "FOREIGN KEY (status_id) REFERENCES statuses (id)"
        << ");";
    qDebug() << SQLiteAdapter::executeSQL(path, createTaskTableRequest.join(' '));

    QStringList avaliableStatuses = {
        "Product Backlog",
        "Sprint Backlog",
        "In Progress",
        "Testing",
        "Done"
    };
    for(auto status : avaliableStatuses)
    {
        QString request = "INSERT INTO statuses (name) VALUES(\""+ status +"\");";
        qDebug() << SQLiteAdapter::executeSQL(path, request);
    }

    /*QStringList teammates = {
        "Xtail"
    };
    for(auto teammate : teammates)
    {
        QString request = "INSERT INTO teammates (name) VALUES(\""+ teammate +"\");";
        qDebug() << SQLiteAdapter::executeSQL(path, request);
    }*/

    QString addFirstTaskRequest = "INSERT INTO tasks (title, status_id, teammate_id) VALUES(\"MyFirstTask\", 1, NULL);";
    qDebug() << SQLiteAdapter::executeSQL(path, addFirstTaskRequest);

    return QFile::exists(path);
}
