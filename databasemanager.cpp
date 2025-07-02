#include "DatabaseManager.h"
#include <QSqlError>
#include <QSqlQuery>
#include <QDebug>
#include "Award.h" // Include Award
#include "Athlete.h" // Include Athlete
#include <QDate>

DatabaseManager* DatabaseManager::m_instance = nullptr;

DatabaseManager& DatabaseManager::getInstance()
{
    if (!m_instance) {
        m_instance = new DatabaseManager();
    }
    return *m_instance;
}

DatabaseManager::DatabaseManager()
{
    // Default database path (can be changed later)
}

DatabaseManager::~DatabaseManager() {
    closeDatabase();
}

bool DatabaseManager::connectToDatabase(const QString& dbPath) {
    m_db = QSqlDatabase::addDatabase("QSQLITE");
    m_db.setDatabaseName(dbPath);

    if (!m_db.open()) {
        qDebug() << "Error: Could not connect to the database: " << m_db.lastError().text();
        emit databaseError(m_db.lastError().text());
        return false;
    }

    qDebug() << "Connected to database: " << dbPath;
    emit databaseConnected();
    return createTables(); // Create tables if they don't exist
}

void DatabaseManager::closeDatabase() {
    if (m_db.isOpen()) {
        m_db.close();
        qDebug() << "Database connection closed.";
    }
}

bool DatabaseManager::createTables()
{
    bool success = true;

    // Create the awards table
    QString awardsTableQuery = R"(
        CREATE TABLE IF NOT EXISTS awards (
            name TEXT NOT NULL,
            date TEXT NOT NULL,
            location TEXT,
            sport INT,
            discipline TEXT,
            level INT,
            place TEXT,
            document TEXT
        );
    )";
    success &= executeQuery(awardsTableQuery);

    // Create the athletes table
    QString athletesTableQuery = R"(
        CREATE TABLE IF NOT EXISTS athletes (
            firstName TEXT NOT NULL,
            lastName TEXT NOT NULL,
            dateOfBirth TEXT,
            email TEXT UNIQUE,
            login TEXT UNIQUE,
            password TEXT
        );
    )";
    success &= executeQuery(athletesTableQuery);

    if (!success) {
        emit databaseError("Failed to create tables");
    }

    return success;
}

bool DatabaseManager::addAthlete(const Athlete& athlete)
{
    QSqlQuery query(m_db);
    query.prepare("INSERT INTO athletes (firstName, lastName, dateOfBirth, email, login, password) "
                  "VALUES (:firstName, :lastName, :dateOfBirth, :email, :login, :password)");

    qDebug() << "firstName: " << athlete.getFirstName();
    qDebug() << "lastName: " << athlete.getLastName();
    qDebug() << "dateOfBirth: " << athlete.getDateOfBirth().toString("yyyy-MM-dd");
    qDebug() << "email: " << athlete.getEmail();
    qDebug() << "login: " << athlete.getLogin();
    qDebug() << "password: " << athlete.getPassword();


    query.bindValue(":firstName", athlete.getFirstName());
    query.bindValue(":lastName", athlete.getLastName());
    query.bindValue(":dateOfBirth", athlete.getDateOfBirth().toString("yyyy-MM-dd"));
    query.bindValue(":email", athlete.getEmail());
    query.bindValue(":login", athlete.getLogin());
    query.bindValue(":password", athlete.getPassword());

    if (!query.exec()) {
        qDebug() << "Error adding athlete:" << query.lastError().text();
        qDebug() << "Query: " << query.lastQuery(); // Add this line
        return false;
    }
    return true;
}

bool DatabaseManager::addAward(const Award& award) {
    QSqlQuery query(m_db);
    query.prepare("INSERT INTO awards (name, date, location, sport, discipline, level, place, document) "
                  "VALUES (:name, :date, :location, :sport, :discipline, :level, :place, :document)");
    query.bindValue(":name", award.getName());
    query.bindValue(":date", award.getDate().toString("yyyy-MM-dd"));
    query.bindValue(":location", award.getLocation());
    query.bindValue(":sport", static_cast<int>(award.getSport()));
    query.bindValue(":discipline", award.getDiscipline());
    query.bindValue(":level", static_cast<int>(award.getLevel()));
    query.bindValue(":place", award.getPlace());
    query.bindValue(":document", award.getDocument());

    if (!query.exec()) {
        qDebug() << "Error adding award:" << query.lastError().text();
        qDebug() << "Query: " << query.lastQuery(); // Add this line for debugging
        return false;
    }
    return true;
}

bool DatabaseManager::updateAward(const Award& award) {
    QSqlQuery query(m_db);
    query.prepare("UPDATE awards SET name = :name, date = :date, location = :location, sport = :sport, "
                  "discipline = :discipline, level = :level, place = :place, document = :document "
                  "WHERE name = :oldName AND date = :oldDate"); //Use unique identifiers
    query.bindValue(":name", award.getName());
    query.bindValue(":date", award.getDate().toString("yyyy-MM-dd"));
    query.bindValue(":location", award.getLocation());
    query.bindValue(":sport", static_cast<int>(award.getSport()));
    query.bindValue(":discipline", award.getDiscipline());
    query.bindValue(":level", static_cast<int>(award.getLevel()));
    query.bindValue(":place", award.getPlace());
    query.bindValue(":document", award.getDocument());
    query.bindValue(":oldName", award.getName()); //TODO: Need unique ID
    query.bindValue(":oldDate", award.getDate().toString("yyyy-MM-dd"));

    if (!query.exec()) {
        qDebug() << "Error updating award:" << query.lastError().text();
        qDebug() << "Query: " << query.lastQuery(); // Add this line for debugging
        return false;
    }
    return true;
}

bool DatabaseManager::removeAward(const Award& award) {
    QSqlQuery query(m_db);
    query.prepare("DELETE FROM awards WHERE name = :name AND date = :date");
    query.bindValue(":name", award.getName());
    query.bindValue(":date", award.getDate().toString("yyyy-MM-dd"));

    if (!query.exec()) {
        qDebug() << "Error removing award:" << query.lastError().text();
        qDebug() << "Query: " << query.lastQuery(); // Add this line for debugging
        return false;
    }
    return true;
}

QList<Award> DatabaseManager::getAllAwards() const {
    QList<Award> awards;
    QSqlQuery query(m_db);
    query.prepare("SELECT name, date, location, sport, discipline, level, place, document FROM awards");

    if (query.exec()) {
        while (query.next()) {
            Award award;
            award.setName(query.value(0).toString());
            award.setDate(QDate::fromString(query.value(1).toString(), "yyyy-MM-dd"));
            award.setLocation(query.value(2).toString());
            award.setSport(static_cast<SportType>(query.value(3).toInt()));
            award.setDiscipline(query.value(4).toString());
            award.setLevel(static_cast<CompetitionLevel>(query.value(5).toInt()));
            award.setPlace(query.value(6).toString());
            award.setDocument(query.value(7).toString());
            awards.append(award);
        }
    } else {
        qDebug() << "Error getting all awards: " << query.lastError().text();
        qDebug() << "Query: " << query.lastQuery(); // Add this line for debugging
    }

    return awards;
}


bool DatabaseManager::updateAthlete(const Athlete& athlete) {
    QSqlQuery query(m_db);
    query.prepare("UPDATE athletes SET firstName = :firstName, lastName = :lastName, dateOfBirth = :dateOfBirth, "
                  "email = :email, password = :password "
                  "WHERE login = :login");
    query.bindValue(":firstName", athlete.getFirstName());
    query.bindValue(":lastName", athlete.getLastName());
    query.bindValue(":dateOfBirth", athlete.getDateOfBirth().toString("yyyy-MM-dd"));
    query.bindValue(":email", athlete.getEmail());
    query.bindValue(":login", athlete.getLogin());
    query.bindValue(":password", athlete.getPassword());

    if (!query.exec()) {
        qDebug() << "Error updating athlete:" << query.lastError().text();
        qDebug() << "Query: " << query.lastQuery(); // Add this line for debugging
        return false;
    }
    return true;
}

bool DatabaseManager::removeAthlete(const Athlete& athlete) {
    QSqlQuery query(m_db);
    query.prepare("DELETE FROM athletes WHERE login = :login");
    query.bindValue(":login", athlete.getLogin());

    if (!query.exec()) {
        qDebug() << "Error removing athlete:" << query.lastError().text();
        qDebug() << "Query: " << query.lastQuery(); // Add this line for debugging
        return false;
    }
    return true;
}

QList<Athlete> DatabaseManager::getAllAthletes() const {
    QList<Athlete> athletes;
    QSqlQuery query(m_db);
    query.prepare("SELECT firstName, lastName, dateOfBirth, email, login, password FROM athletes");

    if (query.exec()) {
        while (query.next()) {
            Athlete athlete;
            athlete.setFirstName(query.value("firstName").toString());
            athlete.setLastName(query.value("lastName").toString());
            athlete.setDateOfBirth(QDate::fromString(query.value("dateOfBirth").toString(), "yyyy-MM-dd"));
            athlete.setEmail(query.value("email").toString());
            athlete.setLogin(query.value("login").toString());
            athlete.setPassword(query.value("password").toString());
            athletes.append(athlete);
        }
    } else {
        qDebug() << "Error getting all athletes: " << query.lastError().text();
        qDebug() << "Query: " << query.lastQuery(); // Add this line for debugging
    }

    return athletes;
}


bool DatabaseManager::executeQuery(const QString& queryStr) {
    QSqlQuery query(m_db);
    query.prepare(queryStr);
    if (!query.exec()) {
        qDebug() << "Error executing query: " << query.lastError().text() << " Query: " << queryStr;
        emit databaseError(query.lastError().text());
        return false;
    }
    return true;
}
