#include "AddAwardDialog.h"
#include "ui_addawarddialog.h"

AddAwardDialog::AddAwardDialog(QWidget *parent, int athleteId) :
    QDialog(parent),
    ui(new Ui::AddAwardDialog),
    m_athleteId(athleteId)
{
    ui->setupUi(this);
}

AddAwardDialog::~AddAwardDialog()
{
    delete ui;
}

Award AddAwardDialog::getAward() const
{
    QString name = ui->nameLineEdit->text();
    QDate date = ui->dateEdit->date();
    QString location = ui->locationLineEdit->text();
    SportType sport = static_cast<SportType>(ui->sportComboBox->currentIndex());
    QString discipline = ui->disciplineLineEdit->text();
    CompetitionLevel level = static_cast<CompetitionLevel>(ui->levelComboBox->currentIndex());
    int place = ui->placeLineEdit->text().toInt();
    QString document = ui->documentLineEdit->text();

    Award award(name, date, location, sport, discipline, level, place, document);
    return award;
}

void AddAwardDialog::fillSportComboBox(const QStringList &sports)
{
    ui->sportComboBox->addItems(sports);
}

void AddAwardDialog::fillLevelComboBox(const QStringList &levels)
{
    ui->levelComboBox->addItems(levels);
}

void AddAwardDialog::on_buttonBox_accepted()
{
    // No changes here, athlete_id is already set in getAward()
}
