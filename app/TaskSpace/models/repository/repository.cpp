#include "repository.h"

Repository::Repository(QObject *parent) :
    QObject(parent),
    m_settingsManager(new SettingsManager()),
    m_databasePath(""),
    m_tasks(QList< QSharedPointer<Task> >())
    //m_tasksTable("tasks")
{
    this->loadSettings();
    this->loadTasksFromDatabase();
    //this->loadMockData();
}

Repository::~Repository()
{
}

QStringList Repository::getAvaliableStatuses()
{
    QStringList avaliableStatuses = {
        "ProductBacklog",
        "SprintBacklog",
        "InProgress",
        "Testing",
        "Done"
    };
    return avaliableStatuses;
}

QList<Task> Repository::getTasks() const
{
    QList<Task> tasks;
    for(auto task : m_tasks)
    {
        tasks.append(*task);
    }
    std::sort(tasks.begin(), tasks.end());
    std::reverse(tasks.begin(), tasks.end());
    return tasks;
}

QList<Task> Repository::getTasks(QString status) const
{
    QList<Task> tasks;
    for(auto task : m_tasks)
    {
        if(task->status() == status)
        {
            tasks.append(*task);
        }
    }
    std::sort(tasks.begin(), tasks.end());
    std::reverse(tasks.begin(), tasks.end());
    return tasks;
}

int Repository::getTaskCountByStatus(QString status)
{
    return this->getTasks(status).length();
}

Task Repository::getTaskByIndex(size_t index) const
{
    try
    {
        return *(this->findTask(index).data());
    }
    catch(std::invalid_argument e)
    {
        qDebug() << "Repository::getTaskByIndex:" << e.what();
        throw e;
    }
}

void Repository::loadSettings()
{
    try
    {
        this->setDatabasePath(m_settingsManager->get("Main", "DBPath").toString());
        this->setCalendarUrl(m_settingsManager->get("Main", "CalendarUrl").toString());
    }
    catch(std::invalid_argument e)
    {
        QMessageBox(QMessageBox::Warning, "Error", e.what()).exec();
        qDebug() << "Repository::loadSettings:" << e.what();
        this->setDatabasePath("");
        this->setCalendarUrl("");
    }
}

void Repository::loadMockData()
{
    QStringList avaliableStatuses = this->getAvaliableStatuses();
    QList<Task> tasks = QList<Task>();
    for(int i = 0; i < 10; i++)
    {
        m_tasks.append(QSharedPointer<Task>(new Task(size_t(i), "example task", avaliableStatuses.first())));
    }
}

bool Repository::initDb(QString path)
{
    return true;
}

QSharedPointer<Task> Repository::findTask(size_t index) const
{
    for(auto task : m_tasks)
    {
        if(task->index() == index)
        {
            return task;
        }
    }

    throw std::invalid_argument("Can not find task with index " + std::to_string(index));
}

/*size_t Repository::getNewTaskIndex()
{
    size_t maxIndex = 0;
    for(auto task : m_tasks)
    {
        maxIndex = std::max(maxIndex, task->index());
    }
    return maxIndex + 1;
}*/

/*void Repository::syncTasksWithDatabase()
{
    Repository::saveTasksToDatabase(m_databasePath, m_tasks);
    this->loadTasksFromDatabase();
    emit this->tasksUpdated();
}*/

/*void Repository::saveTasksToDatabase(const QString &databasePath, const QList<QSharedPointer<Task> > &tasks)
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(databasePath);
    if(!db.open())
    {
        qDebug() << "Repository::saveTasksToDatabase:" << "database is not connected";
        throw std::runtime_error("database is not connected");
    }

    QStringList rawRequest;
            rawRequest << "INSERT INTO tasks (title, status, updated_at, description,"
                    << "due_to_date_enabled, due_to_date, estimated_time, actual_time, archived)"
                    << "VALUES (:title, :status, :updated_at, :description,"
                    << ":due_to_date_enabled, :due_to_date, :estimated_time, :actual_time, :archived);";
    QSqlQuery request;
    qDebug() << request.prepare(rawRequest.join(" "));

    for(auto task : tasks)
    {
        //request.bindValue(":index", QString::number(task->index()));
        request.bindValue(":title", task->title());
        request.bindValue(":status", task->status());
        request.bindValue(":updated_at", task->updatedAt());
        request.bindValue(":description", task->description());
        request.bindValue(":due_to_date_enabled", task->dueToDateEnabled());
        request.bindValue(":due_to_date", task->dueToDate());
        request.bindValue(":estimated_time", task->estimatedTime());
        request.bindValue(":actual_time", task->actualTime());
        request.bindValue(":archived", task->archived());
        request.exec();
    }

    QSqlDatabase::removeDatabase(QSqlDatabase::database().connectionName());
    qDebug() << "AfterDelete" << QSqlDatabase::database().connectionNames();
}*/

void Repository::loadTasksFromDatabase()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(m_databasePath);
    if(!db.open())
    {
        qDebug() << "Repository::saveTasksToDatabase:" << "database is not connected";
        throw std::runtime_error("database is not connected");
    }

    QStringList rawRequest;
            rawRequest << "SELECT * FROM tasks;";
    QSqlQuery request;
    if(!request.prepare(rawRequest.join(" ")))
    {
        QSqlDatabase::removeDatabase(QSqlDatabase::database().connectionName());
        qDebug() << "Repository::loadTasksFromDatabase:" << "can not prepare request";
        throw std::runtime_error("Repository::loadTasksFromDatabase: can not prepare request");
    }

    if(!request.exec())
    {
        QSqlDatabase::removeDatabase(QSqlDatabase::database().connectionName());
        qDebug() << "Repository::loadTasksFromDatabase:" << "can not run request";
        throw std::runtime_error("Repository::loadTasksFromDatabase: can not run request");
    }

    m_tasks.clear();
    while (request.next())
    {
            size_t index = size_t(request.value("id").toUInt());
            QString title = request.value("title").toString();
            QString status = request.value("status").toString();
            QDateTime updatedAt = request.value("updated_at").toDateTime();
            QString description = request.value("description").toString();
            bool dueToDateEnabled = request.value("due_to_date_enabled").toBool();
            QDate dueToDate = request.value("due_to_date").toDate();
            QTime estimatedTime = request.value("estimated_time").toTime();
            QTime actualTime = request.value("actual_time").toTime();

            Task* task = new Task(index, title, status, description);
            task->setUpdatedAt(updatedAt);
            task->setDueToDate(dueToDate);
            task->setDueToDateEnabled(dueToDateEnabled);
            task->setEstimatedTime(estimatedTime);
            task->setActualTime(actualTime);
            m_tasks.append(QSharedPointer<Task>(task));
    }

    emit this->tasksUpdated();
    QSqlDatabase::removeDatabase(QSqlDatabase::database().connectionName());
    qDebug() << "AfterDelete" << QSqlDatabase::database().connectionNames();
}

void Repository::createNewBaseTask()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(m_databasePath);
    if(!db.open())
    {
        qDebug() << "Repository::createNewBaseTask:" << "database is not connected";
        throw std::runtime_error("database is not connected");
    }

    QStringList rawRequest;
            rawRequest << "INSERT INTO tasks (title, status, updated_at, description,"
                    << "due_to_date_enabled, due_to_date, estimated_time, actual_time, archived)"
                    << "VALUES (:title, :status, :updated_at, :description,"
                    << ":due_to_date_enabled, :due_to_date, :estimated_time, :actual_time, :archived);";
    QSqlQuery request;
    if(!request.prepare(rawRequest.join(" ")))
    {
        QSqlDatabase::removeDatabase(QSqlDatabase::database().connectionName());
        qDebug() << "Repository::createNewBaseTask:" << "can not prepare request";
        throw std::runtime_error("Repository::createNewBaseTask: can not prepare request");
    }

    request.bindValue(":title", "BaseTask");
    request.bindValue(":status", this->getAvaliableStatuses().first());
    request.bindValue(":updated_at", QDateTime::currentDateTime());
    request.bindValue(":description", "");
    request.bindValue(":due_to_date_enabled", true);
    request.bindValue(":due_to_date", QDate::currentDate());
    request.bindValue(":estimated_time", QTime());
    request.bindValue(":actual_time", QTime());
    request.bindValue(":archived", false);

    if(!request.exec())
    {
        QSqlDatabase::removeDatabase(QSqlDatabase::database().connectionName());
        qDebug() << "Repository::createNewBaseTask:" << "can not exec request";
        throw std::runtime_error("Repository::createNewBaseTask: can not exec request");
    }

    QSqlDatabase::removeDatabase(QSqlDatabase::database().connectionName());
    this->loadTasksFromDatabase();
}

void Repository::removeTask(size_t index)
{
    /*try
    {
        auto task = this->findTask(index);
        m_tasks.removeAll(task);
        this->syncTasksWithDatabase();
    }
    catch (std::invalid_argument e)
    {
        QMessageBox(QMessageBox::Warning, "Error", e.what()).exec();
        qDebug() << "Repository::removeTask:" << e.what();
    }*/
}

void Repository::updateTaskStatus(size_t index, QString status)
{
    auto task = this->findTask(index);
    task->setStatus(status);

    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(m_databasePath);
    if(!db.open())
    {
        qDebug() << "Repository::createNewBaseTask:" << "database is not connected";
        throw std::runtime_error("database is not connected");
    }

    QStringList rawRequest;
            rawRequest << "UPDATE tasks"
                    << "SET status = " + task->status()
                    << "WHERE tasks.id = " + QString::number(task->index())
                    << ";";

    QSqlQuery request;
    if(!request.prepare(rawRequest.join(" ")))
    {
        QSqlDatabase::removeDatabase(QSqlDatabase::database().connectionName());
        qDebug() << "Repository::updateTaskStatus:" << "can not prepare request"<< rawRequest.join(" ");;
        throw std::runtime_error("Repository::updateTaskStatus: can not prepare request");
    }

    if(!request.exec())
    {
        QSqlDatabase::removeDatabase(QSqlDatabase::database().connectionName());
        qDebug() << "Repository::updateTaskStatus:" << "can not exec request";
        throw std::runtime_error("Repository::updateTaskStatus: can not exec request");
    }

    QSqlDatabase::removeDatabase(QSqlDatabase::database().connectionName());
    this->loadTasksFromDatabase();

    /*try
    {
        auto task = this->findTask(index);
        task->setStatus(status);
        this->syncTasksWithDatabase();
    }
    catch(std::invalid_argument e)
    {
        QMessageBox(QMessageBox::Warning, "Error", e.what()).exec();
        qDebug() << "Repository::updateTaskStatus:" << e.what();
    }*/
}

void Repository::updateTaskInfo(size_t index,
                            QString title,
                            QString description,
                            QDate dueToDate,
                            bool dueToDateEnabled,
                            QTime estimatedTime,
                            QTime actualTime)
{
    /*try
    {
        auto task = this->findTask(index);
        task->setTitle(title);
        task->setDescription(description);
        task->setDueToDate(dueToDate);
        task->setDueToDateEnabled(dueToDateEnabled);
        task->setEstimatedTime(estimatedTime);
        task->setActualTime(actualTime);
        this->syncTasksWithDatabase();
    }
    catch (std::invalid_argument e)
    {
        QMessageBox(QMessageBox::Warning, "Error", e.what()).exec();
        qDebug() << "Repository::updateTaskInfo:" << e.what();
    }*/
}

QString Repository::databasePath() const
{
    return m_databasePath;
}

void Repository::setDatabasePath(const QString &databasePath)
{
    m_databasePath = databasePath;
    emit this->databasePathChanged(m_databasePath);
}

QString Repository::getCalendarUrl() const
{
    return m_calendarUrl;
}

void Repository::setCalendarUrl(const QString &calendarUrl)
{
    m_calendarUrl = calendarUrl;
    emit this->calendarUrlChanged(m_calendarUrl);
}
