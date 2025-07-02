#ifndef DATAMANAGER_H
#define DATAMANAGER_H

#include <QObject>
#include <QList>
#include <QString>

#include "Award.h"
#include "Athlete.h"

class DataManager : public QObject
{
    Q_OBJECT

public:
    static DataManager& getInstance();  // Singleton

    QList<Award> getAwards() const;
    void addAward(const Award& award);
    void updateAward(const Award& award);
    void removeAward(const Award& award);

    QList<Athlete> getAthletes() const;
    void addAthlete(const Athlete& athlete);
    void updateAthlete(const Athlete& athlete);
    void removeAthlete(const Athlete& athlete);

    QList<Award> searchAwards(const QString& searchTerm) const;
    QList<Award> filterAwards(const QString& filterCriteria) const; //TODO: use more sophisticated filter

    bool loadData();
    bool saveData();

signals:
    void awardsChanged();
    void athletesChanged();

private:
    DataManager();
    ~DataManager() override;
    static DataManager* m_instance;

    QList<Award> m_awards;
    QList<Athlete> m_athletes;

    // Helper functions for loading and saving data (implementation details)
    bool loadAwards();
    bool saveAwards();
    bool loadAthletes();
    bool saveAthletes();
};

#endif // DATAMANAGER_H
