#include "Award.h"
#include <QJsonObject>
#include <QJsonValue>
#include <QDebug>

Award::Award() :
    m_id(0),
    m_sport(SportType::Other),
    m_level(CompetitionLevel::Local)
{}

Award::Award(QString name, QDate date, QString location, SportType sport, QString discipline, CompetitionLevel level, int place, QString document) :
    m_name(name),
    m_date(date),
    m_location(location),
    m_sport(sport),
    m_discipline(discipline),
    m_level(level),
    m_place(place),
    m_document(document)
{}

Award::Award(int id, QString name, QDate date, QString location, SportType sport, QString discipline, CompetitionLevel level, int place, QString document) :
    m_id(id),
    m_name(name),
    m_date(date),
    m_location(location),
    m_sport(sport),
    m_discipline(discipline),
    m_level(level),
    m_place(place),
    m_document(document)
{}


int Award::getId() const
{
    return m_id;
}

void Award::setId(int id)
{
    m_id = id;
}

QString Award::getName() const
{
    return m_name;
}

void Award::setName(const QString &name)
{
    m_name = name;
}

QDate Award::getDate() const
{
    return m_date;
}

void Award::setDate(const QDate &date)
{
    m_date = date;
}

QString Award::getLocation() const
{
    return m_location;
}

void Award::setLocation(const QString &location)
{
    m_location = location;
}

SportType Award::getSport() const
{
    return m_sport;
}

void Award::setSport(SportType sport)
{
    m_sport = sport;
}

QString Award::getDiscipline() const
{
    return m_discipline;
}

void Award::setDiscipline(const QString &discipline)
{
    m_discipline = discipline;
}

CompetitionLevel Award::getLevel() const
{
    return m_level;
}

void Award::setLevel(CompetitionLevel level)
{
    m_level = level;
}

int Award::getPlace() const
{
    return m_place;
}

void Award::setPlace(int place)
{
    m_place = place;
}

QString Award::getDocument() const
{
    return m_document;
}

void Award::setDocument(const QString &document)
{
    m_document = document;
}

QJsonObject Award::toJson() const
{
    QJsonObject json;
    json["id"] = m_id;
    json["name"] = m_name;
    json["date"] = m_date.toString("yyyy-MM-dd");
    json["location"] = m_location;
    json["sport"] = static_cast<int>(m_sport);
    json["discipline"] = m_discipline;
    json["level"] = static_cast<int>(m_level);
    json["place"] = m_place;
    json["document"] = m_document;
    return json;
}

Award Award::fromJson(const QJsonObject &json)
{
    Award award;
    award.setId(json["id"].toInt());
    award.setName(json["name"].toString());
    award.setDate(QDate::fromString(json["date"].toString(), "yyyy-MM-dd"));
    award.setLocation(json["location"].toString());
    award.setSport(static_cast<SportType>(json["sport"].toInt()));
    award.setDiscipline(json["discipline"].toString());
    award.setLevel(static_cast<CompetitionLevel>(json["level"].toInt()));
    award.setPlace(json["place"].toInt());
    award.setDocument(json["document"].toString());
    return award;
}
