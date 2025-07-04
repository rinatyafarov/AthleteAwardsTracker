// AthleteProfileWindow.cpp
#include "AthleteProfileWindow.h"
#include "ui_athleteprofilewindow.h"
#include "DatabaseManager.h"
#include <QMessageBox>

AthleteProfileWindow::AthleteProfileWindow(Athlete athlete, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AthleteProfileWindow),
    m_athlete(athlete)
{
    ui->setupUi(this);


    connect(ui->saveButton, &QPushButton::clicked, this, &AthleteProfileWindow::on_saveButton_clicked);


    ui->firstNameLineEdit->setText(m_athlete.getFirstName());
    ui->lastNameLineEdit->setText(m_athlete.getLastName());
    ui->dateOfBirthDateEdit->setDate(m_athlete.getDateOfBirth());
    ui->emailLineEdit->setText(m_athlete.getEmail());
    ui->loginLineEdit->setText(m_athlete.getLogin());

}

AthleteProfileWindow::~AthleteProfileWindow()
{
    delete ui;
}

void AthleteProfileWindow::on_saveButton_clicked()
{

    QString firstName = ui->firstNameLineEdit->text();
    QString lastName = ui->lastNameLineEdit->text();
    QDate dateOfBirth = ui->dateOfBirthDateEdit->date();
    QString email = ui->emailLineEdit->text();
    QString login = ui->loginLineEdit->text();


    Athlete updatedAthlete = m_athlete;
    updatedAthlete.setFirstName(firstName);
    updatedAthlete.setLastName(lastName);
    updatedAthlete.setDateOfBirth(dateOfBirth);
    updatedAthlete.setEmail(email);
    updatedAthlete.setLogin(login);



    DatabaseManager& dbManager = DatabaseManager::getInstance();
    dbManager.updateAthlete(updatedAthlete);


    QMessageBox::information(this, "Успех", "Профиль успешно обновлен.");


    accept();
}
