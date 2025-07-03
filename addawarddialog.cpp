#include "AddAwardDialog.h"
#include "ui_addawarddialog.h"
#include <QMessageBox>

AddAwardDialog::AddAwardDialog(QWidget *parent, int athleteId) :
    QDialog(parent),
    ui(new Ui::AddAwardDialog),
    m_athleteId(athleteId)
{
    ui->setupUi(this);

    // Validate nameLineEdit
    QRegularExpression nameRegex("^[a-zA-Z0-9 ]+$");
    QValidator *nameValidator = new QRegularExpressionValidator(nameRegex, this);
    ui->nameLineEdit->setValidator(nameValidator);

    // Validate placeLineEdit
    QIntValidator *placeValidator = new QIntValidator(0, 10, this);
    ui->placeLineEdit->setValidator(placeValidator);
}

AddAwardDialog::~AddAwardDialog()
{
    delete ui;
}

void AddAwardDialog::on_buttonBox_accepted()
{
    // Get the values from the input fields
    QString name = ui->nameLineEdit->text();
    QDate date = ui->dateEdit->date();
    QString location = ui->locationLineEdit->text();
    SportType sport = static_cast<SportType>(ui->sportComboBox->currentIndex());
    QString discipline = ui->disciplineLineEdit->text();
    CompetitionLevel level = static_cast<CompetitionLevel>(ui->levelComboBox->currentIndex());
    int place = ui->placeLineEdit->text().toInt(); // Convert QString to int
    QString document = ui->documentLineEdit->text();

    // Validate name
    if (name.isEmpty()) {
        QMessageBox::warning(this, "Предупреждение", "Название награды не может быть пустым.");
        return;
    }
    QString validatedName = name;
    int posName = 0;
    QValidator::State nameState = ui->nameLineEdit->validator()->validate(validatedName, posName);
    if (nameState != QValidator::Acceptable) {
        QMessageBox::warning(this, "Предупреждение", "Неверный формат названия награды.");
        return;
    }

    // Validate place
    QString placeText = ui->placeLineEdit->text();
    if (placeText.isEmpty()) {
        QMessageBox::warning(this, "Предупреждение", "Место не может быть пустым.");
        return;
    }
    QString validatedPlace = placeText;
    int posPlace = 0;
    QValidator::State placeState = ui->placeLineEdit->validator()->validate(validatedPlace, posPlace);
    if (placeState != QValidator::Acceptable) {
        QMessageBox::warning(this, "Предупреждение", "Неверный формат места.");
        return;
    }

    // Create an Award object with the validated data
    Award award;
    award.setName(name);
    award.setDate(date);
    award.setLocation(location);
    award.setSport(sport);
    award.setDiscipline(discipline);
    award.setLevel(level);
    award.setPlace(place);
    award.setDocument(document);
    award.setAthleteId(m_athleteId);

    // Set the award to the dialog
    m_award = award;
    accept(); // Accept the dialog if all validations pass
}

Award AddAwardDialog::getAward() const
{
    return m_award;
}

void AddAwardDialog::fillSportComboBox(const QStringList& sports)
{
    ui->sportComboBox->addItems(sports);
}

void AddAwardDialog::fillLevelComboBox(const QStringList& levels)
{
    ui->levelComboBox->addItems(levels);
}
