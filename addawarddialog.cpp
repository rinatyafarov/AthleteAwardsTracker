#include "AddAwardDialog.h"
#include "ui_addawarddialog.h"
#include <QMessageBox>

AddAwardDialog::AddAwardDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddAwardDialog)
{
    ui->setupUi(this);
}

AddAwardDialog::~AddAwardDialog()
{
    delete ui;
}

Award AddAwardDialog::getAward() const
{
    Award award;
    award.setName(ui->nameLineEdit->text());
    award.setDate(ui->dateEdit->date());
    award.setLocation(ui->locationLineEdit->text());
    award.setSport(static_cast<SportType>(ui->sportComboBox->currentIndex()));
    award.setDiscipline(ui->disciplineLineEdit->text());
    award.setLevel(static_cast<CompetitionLevel>(ui->levelComboBox->currentIndex()));
    award.setPlace(ui->placeLineEdit->text());
    award.setDocument(ui->documentLineEdit->text());
    return award;
}

void AddAwardDialog::on_buttonBox_accepted()
{
    // Basic validation
    if (ui->nameLineEdit->text().isEmpty()) {
        QMessageBox::warning(this, "Warning", "Award name cannot be empty.");
        return;
    }
    accept();
}

void AddAwardDialog::fillSportComboBox(const QStringList& sports)
{
    ui->sportComboBox->addItems(sports);
}

void AddAwardDialog::fillLevelComboBox(const QStringList& levels)
{
    ui->levelComboBox->addItems(levels);
}
