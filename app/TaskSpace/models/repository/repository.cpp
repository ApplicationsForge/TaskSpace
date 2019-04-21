#include "repository.h"

Repository::Repository(QObject *parent) :
    QObject(parent),
    m_settingsManager(new SettingsManager()),
    m_storeDirectory(""),
    m_avaliableStatuses(QStringList()),
    m_tasks(QList< Task >()),
    m_calendarUrl("")
{
    this->loadSettings();
    this->loadTasks();
}

Repository::~Repository()
{
}


QList<Task> Repository::getTasks(const QString &filter) const
{
    QList<Task> tasks = Repository::filterTasks(m_tasks, filter);
    std::sort(tasks.begin(), tasks.end());
    std::reverse(tasks.begin(), tasks.end());
    return tasks;
}

QList<Task> Repository::getTasksByStatus(const QString &status, const QString &filter) const
{
    QList<Task> filteredTasks = Repository::filterTasks(m_tasks, filter);
    QList<Task> tasks;
    for(auto task : filteredTasks)
    {
        if(task.status() == status)
        {
            tasks.append(task);
        }
    }
    std::sort(tasks.begin(), tasks.end());
    std::reverse(tasks.begin(), tasks.end());
    return tasks;
}

int Repository::getTaskCountByStatus(QString status)
{
    return this->getTasksByStatus(status).length();
}

Task& Repository::getTaskByIndex(size_t index)
{
    return Repository::findTask(m_tasks, index);
}

void Repository::loadSettings()
{
    try
    {
        this->setStoreDirectory(m_settingsManager->get("Main", "StoreDirectory").toString());
        this->setCalendarUrl(m_settingsManager->get("Main", "CalendarUrl").toString());

        QStringList avaliableStatuses;
        unsigned int avaliableStatusesCount = m_settingsManager->get("Statuses", "Count").toUInt();
        for(unsigned int i = 0; i < avaliableStatusesCount; i++)
        {
            avaliableStatuses.append(m_settingsManager->get("Statuses", "Status" + QString::number(i)).toString());
        }
        this->setAvaliableStatuses(avaliableStatuses);

    }
    catch(std::invalid_argument e)
    {
        QMessageBox(QMessageBox::Warning, "Error", e.what()).exec();
        this->setStoreDirectory(qApp->applicationDirPath());
        this->setCalendarUrl("");
        this->setAvaliableStatuses(QStringList {"ProductBacklog", "SprintBacklog", "InProgress", "Testing", "Done"});
    }
}

void Repository::syncTasks()
{
    this->saveTasks();
    m_tasks.clear();
    this->loadTasks();
}

void Repository::loadTasks()
{
    QString tasksFilePath = Repository::resolveTaskFilePath(m_storeDirectory);
    QFile file(tasksFilePath);
    if(!file.open(QIODevice::ReadWrite | QIODevice::Text))
    {
        qDebug() << "Repository::loadTasks:" << "Can not open file" << file << "for reading";
        throw std::runtime_error("Can not load tasks");
    }

    QString tasksFileContent = QString::fromUtf8(file.readAll());
    file.close();

    QList<Task> tasks = Repository::convertTaskJsonToList(QtJson::parse(tasksFileContent).toList());
    for(auto task : tasks)
    {
        m_tasks.append(task);
    }
    emit this->tasksUpdated();
}

void Repository::saveTasks()
{
    QtJson::JsonArray tasksJson = Repository::convertTaskListToJson(m_tasks);

    QString tasksFilePath = Repository::resolveTaskFilePath(m_storeDirectory);

    QFile file(tasksFilePath);
    if(!file.open(QIODevice::ReadWrite | QIODevice::Text))
    {
        qDebug() << "Repository::saveTasks:" << "Can not open file" << file << "for writing";
        throw std::runtime_error("Can not save tasks");
    }

    // clear file content
    file.resize(0);

    QByteArray serilizedContent = QtJson::serialize(tasksJson);
    QTextStream stream( &file );
    stream << serilizedContent << endl;
    file.close();
}

QtJson::JsonArray Repository::convertTaskListToJson(const QList<Task> &tasks)
{
    QtJson::JsonArray result;
    for(auto task : tasks)
    {
        QtJson::JsonObject taskJsonObject;
        taskJsonObject["index"] = int(task.index());
        taskJsonObject["title"] = task.title();
        taskJsonObject["status"] = task.status();
        taskJsonObject["updated_at"] = task.updatedAt();
        taskJsonObject["description"] = task.description();
        taskJsonObject["due_to_date"] = task.dueToDate();
        taskJsonObject["due_to_date_enabled"] = task.dueToDateEnabled();
        taskJsonObject["estimated_time"] = task.estimatedTime();
        taskJsonObject["actual_time"] = task.actualTime();
        result.push_back(taskJsonObject);
    }
    return result;
}

QList<Task> Repository::convertTaskJsonToList(const QtJson::JsonArray &taskJsonArray)
{
    QList<Task> result = QList<Task>();
    for(auto taskJson : taskJsonArray)
    {
        QMap<QString, QVariant> taskMap = taskJson.toMap();
        size_t index = size_t(taskMap["index"].toUInt());
        QString title = taskMap["title"].toString();
        QString status = taskMap["status"].toString();
        QDateTime updated_at = QDateTime::fromString(taskMap["updated_at"].toString());
        QString description = taskMap["description"].toString();
        QDate dueToDate = QDate::fromString(taskMap["due_to_date"].toString());
        bool dueToDateEnabled = taskMap["due_to_date_enabled"].toBool();
        QTime estimatedTime = taskMap["estimated_time"].toTime();
        QTime actualTime = taskMap["actual_time"].toTime();

        Task task = Task(index, title, status, description);
        task.setUpdatedAt(updated_at);
        task.setDueToDate(dueToDate);
        task.setDueToDateEnabled(dueToDateEnabled);
        task.setEstimatedTime(estimatedTime);
        task.setActualTime(actualTime);
        result.append(task);
    }
    return result;
}

QString Repository::resolveTaskFilePath(const QString &storeDirectory)
{
    return storeDirectory + "/tasks.json";
}

QString Repository::resolveArchiveFilePath(const QString &storeDirectory)
{
    return storeDirectory + "/archive.json";
}

QList<Task> Repository::filterTasks(const QList<Task> &tasks, const QString &filter)
{
    QList<Task> result = {};
    for(auto task : tasks)
    {
        if(filter.isEmpty())
        {
            result.append(task);
            continue;
        }

        if(task.title().contains(filter, Qt::CaseInsensitive))
        {
            result.append(task);
        }
    }
    return result;
}

Task& Repository::findTask(QList<Task> &tasks, size_t index)
{
    for(auto &task : tasks)
    {
        if(task.index() == index)
        {
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
        maxIndex = std::max(maxIndex, task.index());
    }
    return maxIndex + 1;
}

void Repository::setTasks(const QList<Task> &tasks)
{
    m_tasks = tasks;
    emit this->tasksUpdated();
}

Task &Repository::createNewBaseTask()
{
    Task newTask = Task(this->getNewTaskIndex(), "NewBaseTask", this->getAvaliableStatuses().first());
    this->addTask(newTask);
    return this->getTaskByIndex(newTask.index());
}

void Repository::addTask(const Task &task)
{
    m_tasks.append(task);
    this->syncTasks();
}

void Repository::removeTask(size_t index)
{
    try
    {
        auto &task = Repository::findTask(m_tasks, index);
        m_tasks.removeAll(task);
        this->syncTasks();
    }
    catch (std::invalid_argument e)
    {
        QMessageBox(QMessageBox::Warning, "Error", e.what()).exec();
    }
}

void Repository::updateTaskStatus(size_t index, const QString &status)
{
    try
    {
        Task &task = Repository::findTask(m_tasks, index);
        task.setStatus(status);
        this->syncTasks();
    }
    catch(std::invalid_argument e)
    {
        QMessageBox(QMessageBox::Warning, "Error", e.what()).exec();
    }
}

void Repository::updateTaskInfo(size_t index,
                            const QString &title,
                            const QString &description,
                            const QDate &dueToDate,
                            bool dueToDateEnabled,
                            const QTime &estimatedTime,
                            const QTime &actualTime)
{
    try
    {
        auto &task = Repository::findTask(m_tasks, index);
        task.setTitle(title);
        task.setDescription(description);
        task.setDueToDate(dueToDate);
        task.setDueToDateEnabled(dueToDateEnabled);
        task.setEstimatedTime(estimatedTime);
        task.setActualTime(actualTime);
        this->syncTasks();
    }
    catch (std::invalid_argument e)
    {
        QMessageBox(QMessageBox::Warning, "Error", e.what()).exec();
    }
}

QString Repository::storeDirectory() const
{
    return m_storeDirectory;
}

void Repository::setStoreDirectory(const QString &storeDirectory)
{
    m_storeDirectory = storeDirectory;
    m_settingsManager->set("Main", "StoreDirectory", storeDirectory);
    emit this->storeDirectoryChanged(m_storeDirectory);
}

QString Repository::getCalendarUrl() const
{
    return m_calendarUrl;
}

void Repository::setCalendarUrl(const QString &calendarUrl)
{
    m_calendarUrl = calendarUrl;
    m_settingsManager->set("Main", "CalendarUrl", calendarUrl);
    emit this->calendarUrlChanged(m_calendarUrl);
}

QStringList Repository::getAvaliableStatuses()
{
    return m_avaliableStatuses;
}

void Repository::setAvaliableStatuses(const QStringList &avaliableStatuses)
{
    m_avaliableStatuses = avaliableStatuses;

    m_settingsManager->set("Statuses", "Count", m_avaliableStatuses.length());
    for(int i = 0; i < m_avaliableStatuses.length(); i++)
    {
        m_settingsManager->set("Statuses", "Status" + QString::number(i), m_avaliableStatuses[i]);
    }

    emit this->avaliableStatusesChanged(m_avaliableStatuses);
}
