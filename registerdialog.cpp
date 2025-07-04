#include "RegisterDialog.h"
#include "ui_registerdialog.h"
#include "databasemanager.h"
#include "Athlete.h"
#include <QMessageBox>
#include <QCryptographicHash>
#include <QDebug>
#include <QRegularExpressionValidator>

RegisterDialog::RegisterDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::RegisterDialog)
{
    ui->setupUi(this);


    QRegularExpression emailRegex("^[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\\.[a-zA-Z]{2,}$");
    QValidator *emailValidator = new QRegularExpressionValidator(emailRegex, this);
    ui->emailLineEdit->setValidator(emailValidator);
}

RegisterDialog::~RegisterDialog()
{
    delete ui;
}

QString RegisterDialog::getLogin() const
{
    return ui->loginLineEdit->text();
}

QString RegisterDialog::getPassword() const
{
    return ui->passwordLineEdit->text();
}

void RegisterDialog::on_registerButton_clicked()
{
    QString firstName = ui->firstNameLineEdit->text();
    QString lastName = ui->lastNameLineEdit->text();
    QDate dateOfBirth = ui->dateOfBirthDateEdit->date();
    QString email = ui->emailLineEdit->text();
    QString login = ui->loginLineEdit->text();
    QString password = ui->passwordLineEdit->text();


    if (firstName.isEmpty() || lastName.isEmpty() || email.isEmpty() || login.isEmpty() || password.isEmpty()) {
        QMessageBox::warning(this, "Ошибка регистрации", "Заполните все поля.");
        return;
    }


    QString input = email;
    int pos = 0;
    QValidator::State state = ui->emailLineEdit->validator()->validate(input, pos);
    if (state != QValidator::Acceptable) {
        QMessageBox::warning(this, "Ошибка регистрации", "Неккоректный формат email.");
        return;
    }


    if (password.length() < 8) {
        QMessageBox::warning(this, "Ошибка регистрации", "Пароль должен быть больше 8 символов.");
        return;
    }


    QByteArray passwordBytes = password.toUtf8();
    QByteArray hashedPasswordBytes = QCryptographicHash::hash(passwordBytes, QCryptographicHash::Sha256);
    QString hashedPassword = hashedPasswordBytes.toHex();


    DatabaseManager& dbManager = DatabaseManager::getInstance();
    if (dbManager.loginExists(login)) {
        QMessageBox::warning(this, "Ошибка регистрации", "Данный логин уже используется.");
        return;
    }


    Athlete newAthlete(firstName, lastName, dateOfBirth, email, login, hashedPassword);


    bool success = dbManager.addAthlete(newAthlete);
    if (success) {
        QMessageBox::information(this, "Регистрация прошла успешна", "Вы зарегестрированы!");
        qDebug() << "Registration Success, emitting registrationSuccessful()";
        emit registrationSuccessful();
        close();
    } else {
        QMessageBox::critical(this, "Ошибка базы данных", "Ошибка регистрации. Попробуйте позднее.");
    }
}
