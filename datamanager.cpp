#include "DataManager.h"
#include <QList>
#include <QString>
#include <QDebug>
#include "DatabaseManager.h"

DataManager* DataManager::m_instance = nullptr;

DataManager& DataManager::getInstance()
{
    if (!m_instance) {
        m_instance = new DataManager();
    }
    return *m_instance;
}

DataManager::DataManager()
{
    updateAwardList();
}

DataManager::~DataManager() {}

QList<Award> DataManager::getAwards() const
{
    return m_awards;
}

void DataManager::addAward(const Award& award)
{
    DatabaseManager& dbManager = DatabaseManager::getInstance();
    dbManager.addAward(award);
    updateAwardList();
    emit awardsChanged();
}

void DataManager::updateAward(const Award& award)
{
    DatabaseManager& dbManager = DatabaseManager::getInstance();
    dbManager.updateAward(award);
    updateAwardList();
    emit awardsChanged();
}

void DataManager::removeAward(const Award& award)
{
    DatabaseManager& dbManager = DatabaseManager::getInstance();
    dbManager.removeAward(award);
    updateAwardList();
    emit awardsChanged();
}

QList<Athlete> DataManager::getAthletes() const
{
    DatabaseManager& dbManager = DatabaseManager::getInstance();
    return dbManager.getAllAthletes();
}

void DataManager::updateAwardList()
{
    DatabaseManager& dbManager = DatabaseManager::getInstance();
    m_awards = dbManager.getAllAwards();
    emit awardsChanged();
}
