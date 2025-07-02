#ifndef AWARD_H
#define AWARD_H

#include <QDate>
#include <QString>

// Forward declaration
class QJsonObject;

enum class SportType {
    Unknown,
    Athletics,
    Swimming,
    Cycling,
    TeamSport, // Generic for team sports
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
    Award(const QString &name, const QDate &date, const QString &location, SportType sport,
          const QString &discipline, CompetitionLevel level, const QString &place,
          const QString &document);

    QString getName() const;
    void setName(const QString &name);

    QDate getDate() const;
    void setDate(const QDate &date);

    QString getLocation() const;
    void setLocation(const QString &location);

    SportType getSport() const;
    void setSport(const SportType &sport);

    QString getDiscipline() const;
    void setDiscipline(const QString &discipline);

    CompetitionLevel getLevel() const;
    void setLevel(const CompetitionLevel &level);

    QString getPlace() const;
    void setPlace(const QString &place);

    QString getDocument() const;
    void setDocument(const QString &document);

    //JSON support
    QJsonObject toJson() const;
    static Award fromJson(const QJsonObject &json);

    bool operator==(const Award& other) const;

private:
    QString m_name;
    QDate m_date;
    QString m_location;
    SportType m_sport;
    QString m_discipline;
    CompetitionLevel m_level;
    QString m_place;
    QString m_document;
};

#endif // AWARD_H
