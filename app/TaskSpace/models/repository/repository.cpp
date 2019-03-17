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

QString Repository::helloString() const
{
    return m_helloString;
}

void Repository::setHelloString(const QString &helloString)
{
    m_helloString = helloString;
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

QSqlDatabase Repository::openDb(QString path)
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(path);
    if(!db.open())
    {
       db = Repository::initDb(path);
    }
    return db;
}

QSqlDatabase Repository::initDb(QString path)
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(path);

    QStringList createTeammatesTableRequest;
    createTeammatesTableRequest << "CREATE TABLE teammates ("
        << "id INTEGER NOT NULL,"
        << "nickname TEXT NOT NULL,"
        << "name TEXT,"
        << "email TEXT,"
        << "PRIMARY KEY (id));";
    db.exec(createTeammatesTableRequest.join(' '));

    QStringList createTagsTableRequest;
    createTagsTableRequest << "CREATE TABLE tags ("
       << "id INTEGER NOT NULL,"
       << "name TEXT NOT NULL,"
       << "PRIMARY KEY (id));";
    db.exec(createTagsTableRequest.join(' '));

    QStringList createStatusesTableRequest;
    createStatusesTableRequest << "CREATE TABLE statuses ("
       << "id INTEGER NOT NULL,"
       << "name TEXT NOT NULL,"
       << "PRIMARY KEY (id));";
    db.exec(createStatusesTableRequest.join(' '));

    QStringList createTaskTableRequest;
    createTaskTableRequest << "CREATE TABLE tasks ("
        << "id INTEGER NOT NULL,"
        << "title TEXT NOT NULL,"
        << "status_id INTEGER NOT NULL,"
        << "gue_to DATE,"
        << "description TEXT,"
        << "tags TEXT,"
        << "teammate_id INTEGER,"
        << "PRIMARY KEY (id),"
        << "FOREIGN KEY (teammate_id) REFERENCES teammates (id),"
        << "FOREIGN KEY (status_id) REFERENCES statuses (id));";
    db.exec(createTaskTableRequest.join(' '));

    return db;
}
