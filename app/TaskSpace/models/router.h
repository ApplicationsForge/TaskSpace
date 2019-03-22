#ifndef ROUTER_H
#define ROUTER_H

#include <QObject>

#include "models/repository/repository.h"

// Router& router = Router::getInstance();

class Router : public QObject
{
    Q_OBJECT
public:
    static Router& getInstance();
    ~Router();

     Repository* getRepository();

private:
    explicit Router(QObject *parent = nullptr);

    /// Репозиторий, хранящий текущее состояние
    QScopedPointer<Repository> m_repository;

    /**
     * @brief Подключет слоты к сигналам полей класса
     */
    void setupConnections();

    /**
     * @brief Отключает слоты от сигналов полей класса
     */
    void resetConnections();

signals:
    void tasksUpdated();
    void dbPathChanged(QString path);
    void calendarUrlChanged(QString url);

public slots:
    void onRepository_TasksUpdated();
    void onRepository_DbPathChanged(QString path);
    void onRepository_CalendarUrlChanged(QString url);
};

#endif // ROUTER_H
