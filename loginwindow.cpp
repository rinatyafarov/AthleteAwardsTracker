#include "LoginWindow.h"
#include "ui_loginwindow.h"
#include "databasemanager.h" // Import DatabaseManager
#include <QMessageBox>
#include <QCryptographicHash> // For password hashing

LoginWindow::LoginWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LoginWindow)
{
    ui->setupUi(this);
}

LoginWindow::~LoginWindow()
{
    delete ui;
}

void LoginWindow::on_loginButton_clicked() {
    QString login = ui->loginLineEdit->text();
    QString password = ui->passwordLineEdit->text();
    QString hashedPassword = hashPassword(password);

    DatabaseManager& dbManager = DatabaseManager::getInstance();

    // Check if the user exists in the database
    QList<Athlete> athletes = dbManager.getAllAthletes();
    Athlete loggedInAthlete;
    bool found = false;
    for(const Athlete& athlete : athletes){
        if(athlete.getLogin() == login && athlete.getPassword() == hashedPassword) {
            loggedInAthlete = athlete;
            found = true;
            break;
        }
    }


    if (found) {
        emit loginSuccessful(login);
        accept(); // Close the dialog
        ui->errorLabel->setText(""); // Clear any previous error message
    } else {
        ui->errorLabel->setText("Неверный логин или пароль.");
    }
}

QString LoginWindow::hashPassword(const QString& password) const {
    QByteArray passwordBytes = password.toUtf8();
    QByteArray hashedPasswordBytes = QCryptographicHash::hash(passwordBytes, QCryptographicHash::Sha256);
    QString hashedPassword = hashedPasswordBytes.toHex();
    return hashedPassword;
}
