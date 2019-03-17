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
              m_repository.data()->getAvaliableStatuses().last());
    m_repository->addTask(task);
}

void Router::setupConnections()
{
    QObject::connect(m_repository.data(), SIGNAL(tasksUpdated()), this, SLOT(onRepository_TaskUpdated()));
}

void Router::resetConnections()
{
    QObject::disconnect(m_repository.data(), SIGNAL(tasksUpdated()), this, SLOT(onRepository_TaskUpdated()));
}

void Router::onRepository_TaskUpdated()
{
    emit this->tasksUpdated();
}

