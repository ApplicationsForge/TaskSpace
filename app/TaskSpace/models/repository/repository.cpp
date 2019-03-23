#include "repository.h"

Repository::Repository(QObject *parent) :
    QObject(parent),
    m_settingsManager(new SettingsManager()),
    m_databasePath(""),
    m_tasks(QList< QSharedPointer<Task> >())
{
    this->loadSettings();
    m_tasks = Repository::reloadTasksFromDatabase(m_databasePath);
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
    try {
        this->setDatabasePath(m_settingsManager->get("Main", "DBPath").toString());
        this->setCalendarUrl(m_settingsManager->get("Main", "CalendarUrl").toString());
    }
    catch(std::invalid_argument e) {
        QMessageBox(QMessageBox::Warning, "Error", e.what()).exec();
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
    for(auto task : m_tasks) {
        if(task->index() == index) {
            return task;
        }
    }

    throw std::invalid_argument("Can not find task with index " + std::to_string(index));
}

size_t Repository::getNewTaskIndex()
{
    size_t maxIndex = 0;
    for(auto task : m_tasks)
    {
        maxIndex = std::max(maxIndex, task->index());
    }
    return maxIndex + 1;
}

void Repository::syncTasksWithDatabase()
{
    Repository::saveTasksToDatabase(m_databasePath, m_tasks);
    m_tasks.clear();
    m_tasks = Repository::reloadTasksFromDatabase(m_databasePath);
    emit this->tasksUpdated();
}

void Repository::saveTasksToDatabase(const QString &databasePath, const QList<QSharedPointer<Task> > &tasks)
{
    return;
}

QList<QSharedPointer<Task> > Repository::reloadTasksFromDatabase(const QString &databasePath)
{
    return QList<QSharedPointer<Task> >();
}

Task Repository::createNewBaseTask()
{
    try
    {
        Task newTask = Task(this->getNewTaskIndex(), "NewBaseTask", this->getAvaliableStatuses().first());
        this->addTask(newTask);
        return this->getTaskByIndex(newTask.index());
    }
    catch(std::invalid_argument e)
    {
        qDebug() << "Repository::createNewBaseTask:" << e.what();
        throw;
    }
}

void Repository::addTask(Task task)
{
    m_tasks.append(QSharedPointer<Task>(new Task(task.index(), task.title(), task.status())));
    this->syncTasksWithDatabase();
}

void Repository::removeTask(size_t index)
{
    try
    {
        auto task = this->findTask(index);
        m_tasks.removeAll(task);
        this->syncTasksWithDatabase();
    }
    catch (std::invalid_argument e)
    {
        QMessageBox(QMessageBox::Warning, "Error", e.what()).exec();
    }
}

void Repository::updateTaskStatus(size_t index, QString status)
{
    try
    {
        auto task = this->findTask(index);
        task->setStatus(status);
        this->syncTasksWithDatabase();
    }
    catch(std::invalid_argument e)
    {
        QMessageBox(QMessageBox::Warning, "Error", e.what()).exec();
    }
}

void Repository::updateTaskInfo(size_t index,
                            QString title,
                            QString description,
                            QDate dueToDate,
                            bool dueToDateEnabled,
                            QTime estimatedTime,
                            QTime actualTime)
{
    try
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
    }
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
