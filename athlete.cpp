#include "Athlete.h"

Athlete::Athlete() {}

Athlete::Athlete(QString firstName, QString lastName, QDate dateOfBirth, QString email, QString login, QString password) :
    m_firstName(firstName),
    m_lastName(lastName),
    m_dateOfBirth(dateOfBirth),
    m_email(email),
    m_login(login),
    m_password(password)
{}

int Athlete::getId() const
{
    return m_id;
}

void Athlete::setId(int id)
{
    m_id = id;
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
