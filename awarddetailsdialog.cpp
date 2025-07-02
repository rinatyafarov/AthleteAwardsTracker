#include "AwardDetailsDialog.h"
#include "ui_awarddetailsdialog.h"

AwardDetailsDialog::AwardDetailsDialog(const Award& award, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AwardDetailsDialog),
    m_award(award)
{
    ui->setupUi(this);

    // Populate the UI with award details
    ui->nameLabel->setText(m_award.getName());
    ui->dateLabel->setText(m_award.getDate().toString("dd.MM.yyyy"));
    ui->locationLabel->setText(m_award.getLocation());
    ui->sportLabel->setText(QString::number(static_cast<int>(m_award.getSport()))); // Convert enum to string
    ui->disciplineLabel->setText(m_award.getDiscipline());
    ui->levelLabel->setText(QString::number(static_cast<int>(m_award.getLevel()))); // Convert enum to string
    ui->placeLabel->setText(m_award.getPlace());
    ui->documentLabel->setText(m_award.getDocument());
}

AwardDetailsDialog::~AwardDetailsDialog()
{
    delete ui;
}
