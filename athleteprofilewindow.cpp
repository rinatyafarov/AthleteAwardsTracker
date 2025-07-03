// AthleteProfileWindow.cpp
#include "AthleteProfileWindow.h"
#include "ui_athleteprofilewindow.h"
#include "DatabaseManager.h"
#include <QMessageBox>

AthleteProfileWindow::AthleteProfileWindow(Athlete athlete, QWidget *parent) : // QWidget *parent = nullptr
    QDialog(parent), // Pass the parent to the base class constructor
    ui(new Ui::AthleteProfileWindow),
    m_athlete(athlete)
{
    ui->setupUi(this);

    // Connect the button's clicked signal to the on_saveButton_clicked slot
    connect(ui->saveButton, &QPushButton::clicked, this, &AthleteProfileWindow::on_saveButton_clicked);

    //  Отображаем текущие данные спортсмена
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
    // 1. Get data from input fields
    QString firstName = ui->firstNameLineEdit->text();
    QString lastName = ui->lastNameLineEdit->text();
    QDate dateOfBirth = ui->dateOfBirthDateEdit->date();
    QString email = ui->emailLineEdit->text();
    QString login = ui->loginLineEdit->text();

    // 2. Create a new Athlete object with the updated data
    Athlete updatedAthlete = m_athlete; // Copy the current athlete
    updatedAthlete.setFirstName(firstName);
    updatedAthlete.setLastName(lastName);
    updatedAthlete.setDateOfBirth(dateOfBirth);
    updatedAthlete.setEmail(email);
    updatedAthlete.setLogin(login);
    // Password is not changed

    // 3. Update data in the database
    DatabaseManager& dbManager = DatabaseManager::getInstance();
    dbManager.updateAthlete(updatedAthlete);

    // 4. Display a success message
    QMessageBox::information(this, "Успех", "Профиль успешно обновлен.");

    // 5. Close the window
    accept();
}
