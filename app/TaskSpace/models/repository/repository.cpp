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

void Repository::setDbPath(const QString &dbPath)
{
    m_dbPath = dbPath;
    emit this->dbPathChanged(m_dbPath);
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
    return tasks;
}

void Repository::setTasks(const QList< QSharedPointer<Task> > &tasks)
{
    m_tasks = tasks;
    emit this->tasksUpdated();
}

void Repository::addTask(Task task)
{
    m_tasks.append(QSharedPointer<Task>(new Task(task.index(), task.title(), task.status())));
    emit this->tasksUpdated();
}

void Repository::changeTaskStatus(size_t taskIndex, QString status)
{
    if(int(taskIndex) < m_tasks.size())
    {
        m_tasks.at(int(taskIndex))->setStatus(status);
        emit this->tasksUpdated();
    }
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
