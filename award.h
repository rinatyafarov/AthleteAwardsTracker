#ifndef AWARD_H
#define AWARD_H

#include <QString>
#include <QDate>
#include <QJsonObject>
#include <QJsonValue>

enum class SportType {
    Athletics,
    Swimming,
    Cycling,
    TeamSport,
    Football,
    Basketball,
    Volleyball,
    Tennis,
    Boxing,
    Other
};

enum class CompetitionLevel {
    Local,
    Regional,
    National,
    International,
    Olympic
};

class Award
{
public:
    Award();
    Award(QString name, QDate date, QString location, SportType sport, QString discipline, CompetitionLevel level, int place, QString document);
    Award(int id, QString name, QDate date, QString location, SportType sport, QString discipline, CompetitionLevel level, int place, QString document);

    int getId() const;
    void setId(int id);
    QString getName() const;
    void setName(const QString &name);
    QDate getDate() const;
    void setDate(const QDate &date);
    QString getLocation() const;
    void setLocation(const QString &location);
    SportType getSport() const;
    void setSport(SportType sport);
    QString getDiscipline() const;
    void setDiscipline(const QString &discipline);
    CompetitionLevel getLevel() const;
    void setLevel(CompetitionLevel level);
    int getPlace() const;
    void setPlace(int place);
    QString getDocument() const;
    void setDocument(const QString &document);
    QJsonObject toJson() const;
    static Award fromJson(const QJsonObject &json);

    int getAthleteId() const { return m_athleteId; }
    void setAthleteId(int athleteId) { m_athleteId = athleteId; }



private:
    int m_id;
    QString m_name;
    QDate m_date;
    QString m_location;
    SportType m_sport;
    QString m_discipline;
    CompetitionLevel m_level;
    int m_place;
    QString m_document;\
    int m_athleteId;
};

#endif
