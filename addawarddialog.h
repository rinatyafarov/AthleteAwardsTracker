#ifndef ADDAWARDDIALOG_H
#define ADDAWARDDIALOG_H

#include <QDialog>
#include "Award.h"
#include <QStringList>

namespace Ui {
class AddAwardDialog;
}

class AddAwardDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AddAwardDialog(QWidget *parent = nullptr, int athleteId = -1);
    ~AddAwardDialog();

    Award getAward() const;
    void fillSportComboBox(const QStringList& sports);
    void fillLevelComboBox(const QStringList& levels);

private slots:
    void on_buttonBox_accepted();

private:
    Ui::AddAwardDialog *ui;
    Award m_award;
    int m_athleteId;
};

#endif
