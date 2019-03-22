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

void Router::setupConnections()
{
    QObject::connect(m_repository.data(), SIGNAL(tasksUpdated()), this, SLOT(onRepository_TasksUpdated()));
    QObject::connect(m_repository.data(), SIGNAL(dbPathChanged(QString)), this, SLOT(onRepository_DbPathChanged(QString)));
    QObject::connect(m_repository.data(), SIGNAL(calendarUrlChanged(QString)), this, SLOT(onRepository_CalendarUrlChanged(QString)));
}

void Router::resetConnections()
{
    QObject::disconnect(m_repository.data(), SIGNAL(tasksUpdated()), this, SLOT(onRepository_TasksUpdated()));
    QObject::disconnect(m_repository.data(), SIGNAL(dbPathChanged(QString)), this, SLOT(onRepository_DbPathChanged(QString)));
    QObject::disconnect(m_repository.data(), SIGNAL(calendarUrlChanged(QString)), this, SLOT(onRepository_CalendarUrlChanged(QString)));
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

void Router::onRepository_CalendarUrlChanged(QString url)
{
    emit this->calendarUrlChanged(url);
}
