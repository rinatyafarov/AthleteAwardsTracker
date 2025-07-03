#include "DatabaseManager.h"
#include <QSqlError>
#include <QDebug>
#include <QSqlQuery>

DatabaseManager* DatabaseManager::m_instance = nullptr;

DatabaseManager& DatabaseManager::getInstance()
{
    if (!m_instance) {
        m_instance = new DatabaseManager();
    }
    return *m_instance;
}

DatabaseManager::DatabaseManager() {}

DatabaseManager::~DatabaseManager()
{
    closeDatabase();
}

bool DatabaseManager::connectToDatabase(const QString& dbName)
{
    m_db = QSqlDatabase::addDatabase("QSQLITE");
    m_db.setDatabaseName(dbName);
    if (!m_db.open()) {
        qDebug() << "Error: Could not connect to database: " << m_db.lastError().text();
        return false;
    }
    return initializeTables();
}

void DatabaseManager::closeDatabase()
{
    if (m_db.isOpen()) {
        m_db.close();
    }
}

bool DatabaseManager::initializeTables()
{
    QSqlQuery query(m_db);

    // Create athletes table
    if (!query.exec("CREATE TABLE IF NOT EXISTS athletes ("
                    "id INTEGER PRIMARY KEY AUTOINCREMENT,"
                    "firstName VARCHAR(255) NOT NULL,"
                    "lastName VARCHAR(255) NOT NULL,"
                    "dateOfBirth DATE,"
                    "email VARCHAR(255),"
                    "login VARCHAR(255) NOT NULL UNIQUE,"
                    "password VARCHAR(255) NOT NULL"
                    ")")) {
        qDebug() << "Error creating athletes table: " << query.lastError().text();
        return false;
    }

    // Create awards table
    if (!query.exec("CREATE TABLE IF NOT EXISTS awards ("
                    "id INTEGER PRIMARY KEY AUTOINCREMENT,"
                    "name VARCHAR(255) NOT NULL,"
                    "date DATE,"
                    "location VARCHAR(255),"
                    "sport INTEGER,"
                    "discipline VARCHAR(255),"
                    "level INTEGER,"
                    "place INTEGER,"
                    "document VARCHAR(255),"
                    "athlete_id INTEGER,"
                    "FOREIGN KEY (athlete_id) REFERENCES athletes(id)"
                    ")")) {
        qDebug() << "Error creating awards table: " << query.lastError().text();
        return false;
    }
    return true;
}

QList<Award> DatabaseManager::getAllAwards(int athleteId) const
{
    QList<Award> awards;
    QSqlQuery query(m_db);
    query.prepare("SELECT id, name, date, location, sport, discipline, level, place, document FROM awards WHERE athlete_id = :athleteId");
    query.bindValue(":athleteId", athleteId);
    if (query.exec()) {
        while (query.next()) {
            Award award;
            award.setId(query.value("id").toInt());
            award.setName(query.value("name").toString());
            award.setDate(query.value("date").toDate());
            award.setLocation(query.value("location").toString());
            award.setSport(static_cast<SportType>(query.value("sport").toInt()));
            award.setDiscipline(query.value("discipline").toString());
            award.setLevel(static_cast<CompetitionLevel>(query.value("level").toInt()));
            award.setPlace(query.value("place").toInt());
            award.setDocument(query.value("document").toString());
            awards.append(award);
        }
    } else {
        qDebug() << "Error getting all awards: " << query.lastError().text();
    }
    return awards;
}

void DatabaseManager::addAward(const Award& award)
{
    QSqlQuery query(m_db);
    query.prepare("INSERT INTO awards (name, date, location, sport, discipline, level, place, document, athlete_id) "
                  "VALUES (:name, :date, :location, :sport, :discipline, :level, :place, :document, :athlete_id)");
    query.bindValue(":name", award.getName());
    query.bindValue(":date", award.getDate());
    query.bindValue(":location", award.getLocation());
    query.bindValue(":sport", static_cast<int>(award.getSport()));
    query.bindValue(":discipline", award.getDiscipline());
    query.bindValue(":level", static_cast<int>(award.getLevel()));
    query.bindValue(":place", award.getPlace());
    query.bindValue(":document", award.getDocument());
    // Assuming you have a way to get athlete_id, otherwise use a default value
    query.bindValue(":athlete_id", 1); // Replace with the actual athlete_id
    if (!query.exec()) {
        qDebug() << "Error adding award:" << query.lastError().text();
    }
}

void DatabaseManager::updateAward(const Award& award)
{
    QSqlQuery query(m_db);
    query.prepare("UPDATE awards SET name = :name, date = :date, location = :location, "
                  "sport = :sport, discipline = :discipline, level = :level, "
                  "place = :place, document = :document WHERE id = :id");
    query.bindValue(":id", award.getId()); // Assuming you have getId()
    query.bindValue(":name", award.getName());
    query.bindValue(":date", award.getDate());
    query.bindValue(":location", award.getLocation());
    query.bindValue(":sport", static_cast<int>(award.getSport()));
    query.bindValue(":discipline", award.getDiscipline());
    query.bindValue(":level", static_cast<int>(award.getLevel()));
    query.bindValue(":place", award.getPlace());
    query.bindValue(":document", award.getDocument());

    if (!query.exec()) {
        qDebug() << "Error updating award: " << query.lastError().text();
    }
}

void DatabaseManager::removeAward(const Award& award)
{
    QSqlQuery query(m_db);
    query.prepare("DELETE FROM awards WHERE id = :id");
    query.bindValue(":id", award.getId()); // Assuming you have getId()
    if (!query.exec()) {
        qDebug() << "Error removing award: " << query.lastError().text();
    }
}

QList<Athlete> DatabaseManager::getAllAthletes() const
{
    QList<Athlete> athletes;
    QSqlQuery query("SELECT id, firstName, lastName, dateOfBirth, email, login, password FROM athletes", m_db);
    if (query.exec()) {
        while (query.next()) {
            Athlete athlete;
            athlete.setId(query.value("id").toInt()); // Считываем id как int
            athlete.setFirstName(query.value("firstName").toString());
            athlete.setLastName(query.value("lastName").toString());
            athlete.setDateOfBirth(query.value("dateOfBirth").toDate());
            athlete.setEmail(query.value("email").toString());
            athlete.setLogin(query.value("login").toString());
            athlete.setPassword(query.value("password").toString());
            athletes.append(athlete);
        }
    } else {
        qDebug() << "Error getting all athletes: " << query.lastError().text();
    }
    return athletes;
}

bool DatabaseManager::addAthlete(const Athlete& athlete)
{
    QSqlQuery query(m_db);
    query.prepare("INSERT INTO athletes (firstName, lastName, dateOfBirth, email, login, password) "
                  "VALUES (:firstName, :lastName, :dateOfBirth, :email, :login, :password)");
    query.bindValue(":firstName", athlete.getFirstName());
    query.bindValue(":lastName", athlete.getLastName());
    query.bindValue(":dateOfBirth", athlete.getDateOfBirth());
    query.bindValue(":email", athlete.getEmail());
    query.bindValue(":login", athlete.getLogin());
    query.bindValue(":password", athlete.getPassword());

    if (!query.exec()) {
        qDebug() << "Error adding athlete:" << query.lastError().text();
        return false; // Return false on error
    }
    return true; // Return true on success
}

void DatabaseManager::updateAthlete(const Athlete& athlete)
{
    QSqlQuery query(m_db);
    query.prepare("UPDATE athletes SET firstName = :firstName, lastName = :lastName, "
                  "dateOfBirth = :dateOfBirth, email = :email, login = :login, "
                  "password = :password WHERE id = :id");

    query.bindValue(":id", athlete.getId()); // Assuming you have getId()
    query.bindValue(":firstName", athlete.getFirstName());
    query.bindValue(":lastName", athlete.getLastName());
    query.bindValue(":dateOfBirth", athlete.getDateOfBirth());
    query.bindValue(":email", athlete.getEmail());
    query.bindValue(":login", athlete.getLogin());
    query.bindValue(":password", athlete.getPassword());

    if (!query.exec()) {
        qDebug() << "Error updating athlete: " << query.lastError().text();
    }
}

void DatabaseManager::removeAthlete(const Athlete& athlete)
{
    QSqlQuery query(m_db);
    query.prepare("DELETE FROM athletes WHERE id = :id");
    query.bindValue(":id", athlete.getId()); // Assuming you have getId()

    if (!query.exec()) {
        qDebug() << "Error removing athlete: " << query.lastError().text();
    }
}

bool DatabaseManager::executeQuery(const QString& queryStr) {
    QSqlQuery query(m_db);
    if (!query.exec(queryStr)) {
        qDebug() << "Error executing query: " << query.lastError().text() << " Query: " << queryStr;
        return false;
    }
    return true;
}
