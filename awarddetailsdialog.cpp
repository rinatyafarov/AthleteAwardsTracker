#include "AwardDetailsDialog.h"
#include "ui_awarddetailsdialog.h"
#include "MainWindow.h" // Include MainWindow

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
    ui->sportLabel->setText(MainWindow::sportTypeToString(m_award.getSport())); // Use sportTypeToString
    ui->disciplineLabel->setText(m_award.getDiscipline());
    ui->levelLabel->setText(MainWindow::competitionLevelToString(m_award.getLevel())); // Use competitionLevelToString
    ui->placeLabel->setText(QString::number(m_award.getPlace())); // Convert int to QString
    ui->documentLabel->setText(m_award.getDocument());
}

AwardDetailsDialog::~AwardDetailsDialog()
{
    delete ui;
}
