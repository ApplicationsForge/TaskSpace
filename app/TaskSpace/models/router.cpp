#include "router.h"

Router::Router(QObject *parent) :
    QObject(parent),
    m_repository(new Repository(this))
{
    setupConnections();
}

Router::~Router()
{
    resetConnections();
}

Router &Router::getInstance()
{
    static QScopedPointer<Router> m_instance;
    if(m_instance.data() == nullptr)
    {
        m_instance.reset( new Router() );
    }
    return *m_instance;
}

Repository *Router::getRepository()
{
    return m_repository.data();
}

void Router::addExampleTask()
{
    Task task(size_t(m_repository.data()->m_tasks.length()),
              "add example task",
              m_repository.data()->getAvaliableStatuses().first());
    m_repository->addTask(task);
}

void Router::changeTaskStatus(size_t taskIndex, QString status)
{
    m_repository->updateTaskStatus(taskIndex, status);
}

void Router::createNewTask(QString title, QString description)
{
    qDebug() << "Router::createNewTask" << title << description;
}

void Router::updateTask(size_t index,
                        QString title,
                        QString description,
                        QDate dueToDate,
                        bool dueToDateEnabled,
                        QTime estimatedTime,
                        QTime actualTime)
{
    m_repository->updateTaskInfo(index, title, description, dueToDate, dueToDateEnabled, estimatedTime, actualTime);
}

void Router::removeTask(size_t index)
{
    m_repository->removeTask(index);
}

void Router::setupConnections()
{
    QObject::connect(m_repository.data(), SIGNAL(tasksUpdated()), this, SLOT(onRepository_TasksUpdated()));
    QObject::connect(m_repository.data(), SIGNAL(dbPathChanged(QString)), this, SLOT(onRepository_DbPathChanged(QString)));

}

void Router::resetConnections()
{
    QObject::disconnect(m_repository.data(), SIGNAL(tasksUpdated()), this, SLOT(onRepository_TasksUpdated()));
    QObject::disconnect(m_repository.data(), SIGNAL(dbPathChanged(QString)), this, SLOT(onRepository_DbPathChanged(QString)));
}

void Router::onRepository_TasksUpdated()
{
    //qDebug() << "Router::onRepository_TasksUpdated";
    emit this->tasksUpdated();
}

void Router::onRepository_DbPathChanged(QString path)
{
    emit this->dbPathChanged(path);
}
