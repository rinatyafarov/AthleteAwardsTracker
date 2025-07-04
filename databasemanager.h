#ifndef DATABANAGER_H
#define DATABANAGER_H

#include <QObject>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>

#include "Award.h"
#include "Athlete.h"

class DatabaseManager : public QObject
{
    Q_OBJECT

public:
    static DatabaseManager& getInstance();

    bool connectToDatabase(const QString& dbName);
    void closeDatabase();

    QList<Award> getAllAwards(int athleteId) const;
    void addAward(const Award& award);
    void updateAward(const Award& award);
    void removeAward(const Award& award);

    QList<Athlete> getAllAthletes() const;
    bool addAthlete(const Athlete& athlete);
    void updateAthlete(const Athlete& athlete);
    void removeAthlete(const Athlete& athlete);

    QSqlDatabase& getDatabase() { return m_db; }

    bool loginExists(const QString& login);

private:
    DatabaseManager();
    ~DatabaseManager() override;
    static DatabaseManager* m_instance;

    QSqlDatabase m_db;
    bool executeQuery(const QString& queryStr);
    bool initializeTables();

};

#endif
