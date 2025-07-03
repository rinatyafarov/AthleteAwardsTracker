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
    ui->placeLineEdit->setText(QString::number(m_award.getPlace()));
    ui->documentLineEdit->setText(m_award.getDocument());

    // Validate nameLineEdit
    QRegularExpression nameRegex("^[a-zA-Z0-9 ]+$");
    QValidator *nameValidator = new QRegularExpressionValidator(nameRegex, this);
    ui->nameLineEdit->setValidator(nameValidator);

    // Validate placeLineEdit
    QIntValidator *placeValidator = new QIntValidator(0, 10, this);
    ui->placeLineEdit->setValidator(placeValidator);
}

EditAwardDialog::~EditAwardDialog()
{
    delete ui;
}

Award EditAwardDialog::getAward() const
{
    return m_award;
}

void EditAwardDialog::on_buttonBox_accepted()
{
    QString name = ui->nameLineEdit->text();
    QString placeText = ui->placeLineEdit->text();

    QString validatedName = name;
    int posName = 0;
    QValidator::State nameState = ui->nameLineEdit->validator()->validate(validatedName, posName);

    QString validatedPlaceText = placeText;
    int posPlaceText = 0;
    QValidator::State placeState = ui->placeLineEdit->validator()->validate(validatedPlaceText, posPlaceText);

    if (name.isEmpty()) {
        QMessageBox::warning(this, "Предупреждение", "Название награды не может быть пустым.");
        return;
    }

    if (placeText.isEmpty()) {
        QMessageBox::warning(this, "Предупреждение", "Место не может быть пустым.");
        return;
    }

    if (nameState != QValidator::Acceptable) {
        QMessageBox::warning(this, "Предупреждение", "Неверный формат названия награды.");
        return;
    }

    if (placeState != QValidator::Acceptable) {
        QMessageBox::warning(this, "Предупреждение", "Неверный формат места.");
        return;
    }

    // Update the award data
    m_award.setName(name);
    m_award.setPlace(placeText.toInt()); // Convert QString to int

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
