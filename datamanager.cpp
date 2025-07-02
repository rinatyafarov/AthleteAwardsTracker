#include "DataManager.h"
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject> // Add this line
#include <QFile>
#include <QDebug> // For error messages

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
    // Load data on startup
    loadData();
}

DataManager::~DataManager() {
    saveData(); // Save data on exit
}

QList<Award> DataManager::getAwards() const
{
    return m_awards;
}

void DataManager::addAward(const Award& award)
{
    m_awards.append(award);
    emit awardsChanged();
}

void DataManager::updateAward(const Award& award)
{
    // Find the award and update it
    for (int i = 0; i < m_awards.size(); ++i) {
        if (m_awards[i].getName() == award.getName() && m_awards[i].getDate() == award.getDate()) {
            m_awards[i] = award;
            emit awardsChanged();
            return;
        }
    }
    qWarning() << "Award not found for update: " << award.getName();
}

void DataManager::removeAward(const Award& award)
{
    for (auto it = m_awards.begin(); it != m_awards.end(); ) {
        if (it->getName() == award.getName() && it->getDate() == award.getDate()) {
            it = m_awards.erase(it); // erase returns the next iterator
        } else {
            ++it;
        }
    }
    emit awardsChanged();
}

QList<Athlete> DataManager::getAthletes() const
{
    return m_athletes;
}

void DataManager::addAthlete(const Athlete& athlete)
{
    m_athletes.append(athlete);
    emit athletesChanged();
}

void DataManager::updateAthlete(const Athlete& athlete)
{
    // Find the athlete and update it
    for (int i = 0; i < m_athletes.size(); ++i) {
        if (m_athletes[i].getLogin() == athlete.getLogin()) {
            m_athletes[i] = athlete;
            emit athletesChanged();
            return;
        }
    }
    qWarning() << "Athlete not found for update: " << athlete.getLogin();
}

void DataManager::removeAthlete(const Athlete& athlete)
{
    for (auto it = m_athletes.begin(); it != m_athletes.end(); ) {
        if (it->getLogin() == athlete.getLogin()) { // Use a unique identifier
            it = m_athletes.erase(it); // erase returns the next iterator
        } else {
            ++it;
        }
    }
    emit athletesChanged();
}

QList<Award> DataManager::searchAwards(const QString& searchTerm) const
{
    QList<Award> results;
    for (const Award& award : m_awards) {
        if (award.getName().contains(searchTerm, Qt::CaseInsensitive) ||
            award.getLocation().contains(searchTerm, Qt::CaseInsensitive) ||
            award.getDiscipline().contains(searchTerm, Qt::CaseInsensitive))
        {
            results.append(award);
        }
    }
    return results;
}

QList<Award> DataManager::filterAwards(const QString& filterCriteria) const
{
    // TODO: Implement more sophisticated filtering logic here
    Q_UNUSED(filterCriteria); // Suppress warning about unused parameter
    return m_awards; // For now, return all awards
}

bool DataManager::loadData()
{
    bool awardsLoaded = loadAwards();
    bool athletesLoaded = loadAthletes();
    return awardsLoaded && athletesLoaded;
}

bool DataManager::saveData()
{
    bool awardsSaved = saveAwards();
    bool athletesSaved = saveAthletes();
    return awardsSaved && athletesSaved;
}

bool DataManager::loadAwards() {
    QFile file("awards.json");
    if (!file.open(QIODevice::ReadOnly)) {
        qWarning() << "Couldn't open awards.json for reading.";
        return false;
    }
    QByteArray saveData = file.readAll();
    QJsonDocument loadDoc(QJsonDocument::fromJson(saveData));
    QJsonArray awardArray = loadDoc.array();

    m_awards.clear(); // Clear existing data before loading
    for (int i = 0; i < awardArray.size(); ++i) {
        QJsonObject awardObject = awardArray[i].toObject();
        Award award = Award::fromJson(awardObject);
        m_awards.append(award);
    }

    return true;
}

bool DataManager::saveAwards() {
    QJsonArray awardArray;
    for (const Award& award : m_awards) {
        awardArray.append(award.toJson());
    }

    QJsonDocument saveDoc(awardArray);
    QFile file("awards.json");
    if (!file.open(QIODevice::WriteOnly)) {
        qWarning() << "Couldn't open awards.json for writing.";
        return false;
    }
    file.write(saveDoc.toJson());

    return true;
}

bool DataManager::loadAthletes() {
    QFile file("athletes.json");
    if (!file.open(QIODevice::ReadOnly)) {
        qWarning() << "Couldn't open athletes.json for reading.";
        return false;
    }
    QByteArray saveData = file.readAll();
    QJsonDocument loadDoc(QJsonDocument::fromJson(saveData));
    QJsonArray athleteArray = loadDoc.array();

    m_athletes.clear(); // Clear existing data before loading
    for (int i = 0; i < athleteArray.size(); ++i) {
        QJsonObject athleteObject = athleteArray[i].toObject();
        Athlete athlete = Athlete::fromJson(athleteObject);
        m_athletes.append(athlete);
    }

    return true;
}

bool DataManager::saveAthletes() {
    QJsonArray athleteArray;
    for (const Athlete& athlete : m_athletes) {
        athleteArray.append(athlete.toJson());
    }

    QJsonDocument saveDoc(athleteArray);
    QFile file("athletes.json");
    if (!file.open(QIODevice::WriteOnly)) {
        qWarning() << "Couldn't open athletes.json for writing.";
        return false;
    }
    file.write(saveDoc.toJson());

    return true;
}
