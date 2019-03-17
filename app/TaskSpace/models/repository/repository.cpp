#include "repository.h"

Repository::Repository(QObject *parent) :
    QObject(parent),
    m_settingsManager(new SettingsManager()),
    m_dbPath("")
{
    this->loadSettings();
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
        "Product Backlog",
        "Sprint Backlog",
        "In Progress",
        "Testing",
        "Done"
    };
    return avaliableStatuses;
}

QList<Task> Repository::getMockTasks()
{
    QStringList avaliableStatuses = this->getAvaliableStatuses();
    QList<Task> tasks = QList<Task>();
    for(int i = 0; i < 1000; i++)
    {
        tasks.append(Task(size_t(i), "example task", avaliableStatuses.first()));
    }
    return tasks;
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

bool Repository::initDb(QString path)
{
    return true;
}
