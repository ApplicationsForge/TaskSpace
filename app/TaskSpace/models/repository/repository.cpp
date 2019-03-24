#include "repository.h"

Repository::Repository(QObject *parent) :
    QObject(parent),
    m_settingsManager(new SettingsManager()),
    m_storeDirectory(""),
    m_tasks(QList< QSharedPointer<Task> >())
{
    this->loadSettings();
    this->loadTasks();
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
    return *(this->findTask(index).data());
}

void Repository::loadSettings()
{
    try
    {
        this->setStoreDirectory(m_settingsManager->get("Main", "StoreDirectory").toString());
        this->setCalendarUrl(m_settingsManager->get("Main", "CalendarUrl").toString());
    }
    catch(std::invalid_argument e)
    {
        QMessageBox(QMessageBox::Warning, "Error", e.what()).exec();
        this->setStoreDirectory(qApp->applicationDirPath());
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
        m_tasks.append(QSharedPointer<Task>(new Task(task)));
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

QtJson::JsonArray Repository::convertTaskListToJson(const QList<QSharedPointer<Task> > &tasks)
{
    QtJson::JsonArray result;
    for(auto task : tasks)
    {
        QtJson::JsonObject taskJsonObject;
        taskJsonObject["index"] = int(task->index());
        taskJsonObject["title"] = task->title();
        taskJsonObject["status"] = task->status();
        taskJsonObject["updated_at"] = task->updatedAt();
        taskJsonObject["description"] = task->description();
        taskJsonObject["due_to_date"] = task->dueToDate();
        taskJsonObject["due_to_date_enabled"] = task->dueToDateEnabled();
        taskJsonObject["estimated_time"] = task->estimatedTime();
        taskJsonObject["actual_time"] = task->actualTime();
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
    return storeDirectory + "tasks.json";
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
    Task newTask = Task(this->getNewTaskIndex(), "NewBaseTask", this->getAvaliableStatuses().first());
    this->addTask(newTask);
    return this->getTaskByIndex(newTask.index());
}

void Repository::addTask(Task task)
{
    m_tasks.append(QSharedPointer<Task>(new Task(task.index(), task.title(), task.status())));
    this->syncTasks();
}

void Repository::removeTask(size_t index)
{
    try
    {
        auto task = this->findTask(index);
        m_tasks.removeAll(task);
        this->syncTasks();
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
        this->syncTasks();
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
    emit this->storeDirectoryChanged(m_storeDirectory);
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
