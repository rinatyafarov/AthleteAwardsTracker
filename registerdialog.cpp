#include "RegisterDialog.h"
#include "ui_registerdialog.h"
#include "databasemanager.h" // Include DatabaseManager
#include "Athlete.h" // Include Athlete
#include <QMessageBox>
#include <QCryptographicHash>
#include <QDebug>
#include <QRegularExpressionValidator>

RegisterDialog::RegisterDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::RegisterDialog)
{
    ui->setupUi(this);

    // Добавляем валидатор для email
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

    // Validation
    if (firstName.isEmpty() || lastName.isEmpty() || email.isEmpty() || login.isEmpty() || password.isEmpty()) {
        QMessageBox::warning(this, "Registration Error", "All fields are required.");
        return;
    }

    // Check email validity using the validator
    QString input = email;
    int pos = 0;
    QValidator::State state = ui->emailLineEdit->validator()->validate(input, pos);
    if (state != QValidator::Acceptable) {
        QMessageBox::warning(this, "Registration Error", "Invalid email format.");
        return;
    }

    // Password Validation
    if (password.length() < 8) {
        QMessageBox::warning(this, "Registration Error", "Password must be at least 8 characters long.");
        return;
    }

    // Hash password
    QByteArray passwordBytes = password.toUtf8();
    QByteArray hashedPasswordBytes = QCryptographicHash::hash(passwordBytes, QCryptographicHash::Sha256);
    QString hashedPassword = hashedPasswordBytes.toHex();

    // Check if login already exists
    DatabaseManager& dbManager = DatabaseManager::getInstance();
    if (dbManager.loginExists(login)) {
        QMessageBox::warning(this, "Registration Error", "This login is already taken.");
        return;
    }

    // Create new athlete
    Athlete newAthlete(firstName, lastName, dateOfBirth, email, login, hashedPassword);

    // Add athlete to database
    bool success = dbManager.addAthlete(newAthlete);
    if (success) {
        QMessageBox::information(this, "Registration Successful", "You have successfully registered!");
        qDebug() << "Registration Success, emitting registrationSuccessful()";
        emit registrationSuccessful();  // Emit the signal
        close();  // Close the RegisterDialog
    } else {
        QMessageBox::critical(this, "Database Error", "Failed to register. Please try again.");
    }
}
