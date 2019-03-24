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
    void storeDirectoryChanged(QString path);
    void calendarUrlChanged(QString url);
    void avaliableStatusesChanged(QString statuses);

public slots:
    void onRepository_TasksUpdated();
    void onRepository_StoreDirectoryChanged(QString path);
    void onRepository_CalendarUrlChanged(QString url);
    void onRepository_AvaliableStatusesChanged(QStringList avaliableStatuses);

};

#endif // ROUTER_H
