#include "Athlete.h"
#include <QJsonObject>

Athlete::Athlete()
{
}

Athlete::Athlete(const QString &firstName, const QString &lastName, const QDate &dateOfBirth,
                 const QString &email, const QString &login, const QString &password) :
    m_firstName(firstName),
    m_lastName(lastName),
    m_dateOfBirth(dateOfBirth),
    m_email(email),
    m_login(login),
    m_password(password)
{
}

QString Athlete::getFirstName() const
{
    return m_firstName;
}

void Athlete::setFirstName(const QString &firstName)
{
    m_firstName = firstName;
}

QString Athlete::getLastName() const
{
    return m_lastName;
}

void Athlete::setLastName(const QString &lastName)
{
    m_lastName = lastName;
}

QDate Athlete::getDateOfBirth() const
{
    return m_dateOfBirth;
}

void Athlete::setDateOfBirth(const QDate &dateOfBirth)
{
    m_dateOfBirth = dateOfBirth;
}

QString Athlete::getEmail() const
{
    return m_email;
}

void Athlete::setEmail(const QString &email)
{
    m_email = email;
}

QString Athlete::getLogin() const
{
    return m_login;
}

void Athlete::setLogin(const QString &login)
{
    m_login = login;
}

QString Athlete::getPassword() const
{
    return m_password;
}

void Athlete::setPassword(const QString &password)
{
    m_password = password;
}

QJsonObject Athlete::toJson() const {
    QJsonObject json;
    json["firstName"] = m_firstName;
    json["lastName"] = m_lastName;
    json["dateOfBirth"] = m_dateOfBirth.toString("yyyy-MM-dd");
    json["email"] = m_email;
    json["login"] = m_login;
    json["password"] = m_password; // In real app, hash this!
    return json;
}

Athlete Athlete::fromJson(const QJsonObject &json) {
    Athlete athlete;
    athlete.setFirstName(json["firstName"].toString());
    athlete.setLastName(json["lastName"].toString());
    athlete.setDateOfBirth(QDate::fromString(json["dateOfBirth"].toString(), "yyyy-MM-dd"));
    athlete.setEmail(json["email"].toString());
    athlete.setLogin(json["login"].toString());
    athlete.setPassword(json["password"].toString()); // In real app, NEVER store plain text passwords!
    return athlete;
}
bool Athlete::operator==(const Athlete& other) const
{
    return (m_firstName == other.m_firstName &&
            m_lastName == other.m_lastName &&
            m_dateOfBirth == other.m_dateOfBirth &&
            m_email == other.m_email &&
            m_login == other.m_login &&
            m_password == other.m_password);
}
