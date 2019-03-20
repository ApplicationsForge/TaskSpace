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

     void addExampleTask();

     void changeTaskStatus(size_t taskIndex, QString status);

     void createNewTask(QString title, QString description);

     void updateTask(size_t index, QString title, QString description);

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

public slots:
    void onRepository_TasksUpdated();
};

#endif // ROUTER_H
