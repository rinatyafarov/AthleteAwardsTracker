#ifndef ATHLETE_H
#define ATHLETE_H

#include <QString>
#include <QDate>

// Forward declaration
class QJsonObject;

class Athlete
{
public:
    Athlete();
    Athlete(const QString &firstName, const QString &lastName, const QDate &dateOfBirth,
            const QString &email, const QString &login, const QString &password);

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

    //JSON support
    QJsonObject toJson() const;
    static Athlete fromJson(const QJsonObject &json);

    bool operator==(const Athlete& other) const;


private:
    QString m_firstName;
    QString m_lastName;
    QDate m_dateOfBirth;
    QString m_email;
    QString m_login;
    QString m_password; // TODO: Store password securely (hash + salt)
};

#endif // ATHLETE_H
