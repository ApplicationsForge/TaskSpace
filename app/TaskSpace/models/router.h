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

     Task addNewBaseTask();

     void changeTaskStatus(size_t taskIndex, QString status);

     void createNewTask(QString title, QString description);

     void updateTask(size_t index,
                     QString title,
                     QString description,
                     QDate dueToDate = QDate(),
                     bool dueToDateEnabled = true,
                     QTime estimatedTime = QTime(0, 0),
                     QTime actualTime = QTime(0, 0));

     void removeTask(size_t index);

     void setDbPath(QString path);

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

public slots:
    void onRepository_TasksUpdated();
    void onRepository_DbPathChanged(QString path);
};

#endif // ROUTER_H
