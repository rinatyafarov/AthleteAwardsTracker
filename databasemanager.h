#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <QObject>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

#include "Award.h"
#include "Athlete.h"

class DatabaseManager : public QObject
{
    Q_OBJECT
public:
    static DatabaseManager& getInstance();

    bool connectToDatabase(const QString& dbPath);
    void closeDatabase();

    bool createTables();

    // Award operations
    bool addAward(const Award& award);
    bool updateAward(const Award& award);
    bool removeAward(const Award& award);
    QList<Award> getAllAwards() const;

    // Athlete operations
    bool addAthlete(const Athlete& athlete);
    bool updateAthlete(const Athlete& athlete);
    bool removeAthlete(const Athlete& athlete);
    QList<Athlete> getAllAthletes() const;

signals:
    void databaseError(const QString& error);
    void databaseConnected();

private:
    DatabaseManager();
    ~DatabaseManager() override;
    static DatabaseManager* m_instance;

    QSqlDatabase m_db;

    // Helper functions
    bool executeQuery(const QString& query);

    // Prevent copy construction and assignment
    DatabaseManager(const DatabaseManager&) = delete;
    DatabaseManager& operator=(const DatabaseManager&) = delete;
};

#endif // DATABASEMANAGER_H
