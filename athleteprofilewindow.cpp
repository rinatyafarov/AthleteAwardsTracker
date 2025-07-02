#include "AthleteProfileWindow.h"
#include "ui_athleteprofilewindow.h"

AthleteProfileWindow::AthleteProfileWindow(Athlete athlete, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::AthleteProfileWindow),
    m_athlete(athlete)
{
    ui->setupUi(this);
    populateUI();
}

AthleteProfileWindow::~AthleteProfileWindow()
{
    delete ui;
}

void AthleteProfileWindow::populateUI()
{
    ui->firstNameLineEdit->setText(m_athlete.getFirstName());
    ui->lastNameLineEdit->setText(m_athlete.getLastName());
    ui->dateOfBirthDateEdit->setDate(m_athlete.getDateOfBirth());
    ui->emailLineEdit->setText(m_athlete.getEmail());
    ui->loginLineEdit->setText(m_athlete.getLogin());
    ui->passwordLineEdit->setText(m_athlete.getPassword());
}
