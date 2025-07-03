#include "LoginWindow.h"
#include "ui_loginwindow.h"
#include "RegisterDialog.h" // Include RegisterDialog
#include "databasemanager.h"
#include <QMessageBox>
#include <QCryptographicHash>
#include <QDebug>

LoginWindow::LoginWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LoginWindow)
{
    ui->setupUi(this);
    connect(ui->registerButton, &QPushButton::clicked, this, &LoginWindow::on_registerButton_clicked);
}

LoginWindow::~LoginWindow()
{
    delete ui;
}

void LoginWindow::on_loginButton_clicked()
{
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
        qDebug() << "Login Successful, emitting signal with Athlete: " << loggedInAthlete.getLogin(); // Debugging
        emit loginSuccessful(loggedInAthlete); // Change
        accept();
    } else {
        QMessageBox::warning(this, "Login Failed", "Incorrect login or password.");
    }
}

void LoginWindow::on_registerButton_clicked()
{
    RegisterDialog registerDialog(this);
    connect(&registerDialog, &RegisterDialog::registrationSuccessful, this, &LoginWindow::show);
    registerDialog.exec();
}

QString LoginWindow::hashPassword(const QString& password) const {
    QByteArray passwordBytes = password.toUtf8();
    QByteArray hashedPasswordBytes = QCryptographicHash::hash(passwordBytes, QCryptographicHash::Sha256);
    QString hashedPassword = hashedPasswordBytes.toHex();
    return hashedPassword;
}
