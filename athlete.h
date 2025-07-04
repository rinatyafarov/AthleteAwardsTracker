#ifndef ATHLETE_H
#define ATHLETE_H

#include <QString>
#include <QDate>
#include <QJsonObject>

class Athlete
{
public:
    Athlete();
    Athlete(QString firstName, QString lastName, QDate dateOfBirth, QString email, QString login, QString password);

    int getId() const;
    void setId(int id);
    QString getFirstName() const;
    void setFirstName(const QString &firstName);
    QString getLastName() const;
    void setLastName(const QString &lastName);
    QDate getDateOfBirth() const;
    void setDateOfBirth(const QDate &dateOfBirth);
    QString getEmail() const;
    void setEmail(const QString &email);
    QString getLogin() const;
    void setLogin(const QString &login);
    QString getPassword() const;
    void setPassword(const QString &password);

private:
    int m_id;
    QString m_firstName;
    QString m_lastName;
    QDate m_dateOfBirth;
    QString m_email;
    QString m_login;
    QString m_password;
};

#endif
