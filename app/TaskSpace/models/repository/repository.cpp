#include "repository.h"

Repository::Repository(QObject *parent) :
    QObject(parent),
    m_settingsManager(new SettingsManager()),
    m_dbPath(""),
    m_tasks(QList< QSharedPointer<Task> >())
{
    this->loadSettings();

    this->loadMockData();
}

Repository::~Repository()
{
}

QString Repository::dbPath() const
{
    return m_dbPath;
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
    return *(this->findTask(index).data());
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

void Repository::setTasks(const QList< QSharedPointer<Task> > &tasks)
{
    m_tasks = tasks;
    emit this->tasksUpdated();
}

Task Repository::createNewBaseTask()
{
    Task newTask = Task(this->getNewTaskIndex(), "", this->getAvaliableStatuses().first());
    this->addTask(newTask);
    return this->getTaskByIndex(newTask.index());
}

void Repository::addTask(Task task)
{
    m_tasks.append(QSharedPointer<Task>(new Task(task.index(), task.title(), task.status())));
    emit this->tasksUpdated();
}

void Repository::removeTask(size_t index)
{
    try
    {
        auto task = this->findTask(index);
        m_tasks.removeAll(task);
        emit this->tasksUpdated();
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
        emit this->tasksUpdated();
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
        emit this->tasksUpdated();
    }
    catch (std::invalid_argument e)
    {
        QMessageBox(QMessageBox::Warning, "Error", e.what()).exec();
    }
}

void Repository::setDbPath(const QString &dbPath)
{
    m_dbPath = dbPath;
    emit this->dbPathChanged(m_dbPath);
}
