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

signals:
    void awardsChanged();
    void athletesChanged();

private:
    DataManager();
    ~DataManager() override;
    static DataManager* m_instance;

    QList<Award> m_awards;

    void updateAwardList(); // Method to update the list of awards from the database
};

#endif // DATAMANAGER_H
