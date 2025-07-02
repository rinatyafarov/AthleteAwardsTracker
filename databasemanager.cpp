#include "DatabaseManager.h"

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

bool DatabaseManager::createTables() {
    QString createAwardsTableQuery = "CREATE TABLE IF NOT EXISTS awards ("
                                     "id INTEGER PRIMARY KEY AUTOINCREMENT,"
                                     "name TEXT NOT NULL,"
                                     "date TEXT NOT NULL,"
                                     "location TEXT,"
                                     "sport INTEGER,"
                                     "discipline TEXT,"
                                     "level INTEGER,"
                                     "place TEXT,"
                                     "document TEXT"
                                     ");";

    QString createAthletesTableQuery = "CREATE TABLE IF NOT EXISTS athletes ("
                                       "id INTEGER PRIMARY KEY AUTOINCREMENT,"
                                       "firstName TEXT NOT NULL,"
                                       "lastName TEXT NOT NULL,"
                                       "dateOfBirth TEXT,"
                                       "email TEXT,"
                                       "login TEXT UNIQUE NOT NULL,"
                                       "password TEXT" //TODO: Hash password
                                       ");";

    return executeQuery(createAwardsTableQuery) && executeQuery(createAthletesTableQuery);
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

    return query.exec();
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

    return query.exec();
}

bool DatabaseManager::removeAward(const Award& award) {
    QSqlQuery query(m_db);
    query.prepare("DELETE FROM awards WHERE name = :name AND date = :date");
    query.bindValue(":name", award.getName());
    query.bindValue(":date", award.getDate().toString("yyyy-MM-dd"));

    return query.exec();
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
    }

    return awards;
}

bool DatabaseManager::addAthlete(const Athlete& athlete) {
    QSqlQuery query(m_db);
    query.prepare("INSERT INTO athletes (firstName, lastName, dateOfBirth, email, login, password) "
                  "VALUES (:firstName, :lastName, :dateOfBirth, :email, :login, :password)");
    query.bindValue(":firstName", athlete.getFirstName());
    query.bindValue(":lastName", athlete.getLastName());
    query.bindValue(":dateOfBirth", athlete.getDateOfBirth().toString("yyyy-MM-dd"));
    query.bindValue(":email", athlete.getEmail());
    query.bindValue(":login", athlete.getLogin());
    query.bindValue(":password", athlete.getPassword());

    return query.exec();
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

    return query.exec();
}

bool DatabaseManager::removeAthlete(const Athlete& athlete) {
    QSqlQuery query(m_db);
    query.prepare("DELETE FROM athletes WHERE login = :login");
    query.bindValue(":login", athlete.getLogin());

    return query.exec();
}

QList<Athlete> DatabaseManager::getAllAthletes() const {
    QList<Athlete> athletes;
    QSqlQuery query(m_db);
    query.prepare("SELECT firstName, lastName, dateOfBirth, email, login, password FROM athletes");

    if (query.exec()) {
        while (query.next()) {
            Athlete athlete;
            athlete.setFirstName(query.value(0).toString());
            athlete.setLastName(query.value(1).toString());
            athlete.setDateOfBirth(QDate::fromString(query.value(2).toString(), "yyyy-MM-dd"));
            athlete.setEmail(query.value(3).toString());
            athlete.setLogin(query.value(4).toString());
            athlete.setPassword(query.value(5).toString());
            athletes.append(athlete);
        }
    } else {
        qDebug() << "Error getting all athletes: " << query.lastError().text();
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
