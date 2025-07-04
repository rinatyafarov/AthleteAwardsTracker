#include "AwardDetailsDialog.h"
#include "ui_awarddetailsdialog.h"
#include "MainWindow.h"

AwardDetailsDialog::AwardDetailsDialog(const Award& award, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AwardDetailsDialog),
    m_award(award)
{
    ui->setupUi(this);


    ui->nameLabel->setText(m_award.getName());
    ui->dateLabel->setText(m_award.getDate().toString("dd.MM.yyyy"));
    ui->locationLabel->setText(m_award.getLocation());
    ui->sportLabel->setText(MainWindow::sportTypeToString(m_award.getSport()));
    ui->disciplineLabel->setText(m_award.getDiscipline());
    ui->levelLabel->setText(MainWindow::competitionLevelToString(m_award.getLevel()));
    ui->placeLabel->setText(QString::number(m_award.getPlace()));
    ui->documentLabel->setText(m_award.getDocument());
}

AwardDetailsDialog::~AwardDetailsDialog()
{
    delete ui;
}
