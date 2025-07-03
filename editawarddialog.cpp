#include "EditAwardDialog.h"
#include "ui_editawarddialog.h"
#include <QMessageBox>

EditAwardDialog::EditAwardDialog(const Award &award, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::EditAwardDialog),
    m_award(award)
{
    ui->setupUi(this);

    // Initialize the UI with the existing award data
    ui->nameLineEdit->setText(m_award.getName());
    ui->dateEdit->setDate(m_award.getDate());
    ui->locationLineEdit->setText(m_award.getLocation());
    ui->sportComboBox->setCurrentIndex(static_cast<int>(m_award.getSport()));
    ui->disciplineLineEdit->setText(m_award.getDiscipline());
    ui->levelComboBox->setCurrentIndex(static_cast<int>(m_award.getLevel()));
    ui->placeLineEdit->setText(QString::number(m_award.getPlace())); // Convert int to QString
    ui->documentLineEdit->setText(m_award.getDocument());
}

EditAwardDialog::~EditAwardDialog()
{
    delete ui;
}

Award EditAwardDialog::getAward() const
{
    Award award;
    award.setName(ui->nameLineEdit->text());
    award.setDate(ui->dateEdit->date());
    award.setLocation(ui->locationLineEdit->text());
    award.setSport(static_cast<SportType>(ui->sportComboBox->currentIndex()));
    award.setDiscipline(ui->disciplineLineEdit->text());
    award.setLevel(static_cast<CompetitionLevel>(ui->levelComboBox->currentIndex()));
    award.setPlace(ui->placeLineEdit->text().toInt()); // Convert QString to int
    award.setDocument(ui->documentLineEdit->text());
    return award;
}

void EditAwardDialog::on_buttonBox_accepted()
{
    // Basic validation
    if (ui->nameLineEdit->text().isEmpty()) {
        QMessageBox::warning(this, "Warning", "Award name cannot be empty.");
        return;
    }
    accept();
}

void EditAwardDialog::fillSportComboBox(const QStringList& sports)
{
    ui->sportComboBox->addItems(sports);
}

void EditAwardDialog::fillLevelComboBox(const QStringList& levels)
{
    ui->levelComboBox->addItems(levels);
}
