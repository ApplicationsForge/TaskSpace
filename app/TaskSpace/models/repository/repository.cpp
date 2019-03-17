#include "repository.h"

Repository::Repository(QObject *parent) :
    QObject(parent),
    m_settingsManager(new SettingsManager()),
    m_dbPath("")
{
    this->loadSettings();
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
