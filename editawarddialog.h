#ifndef EDITAWARDDIALOG_H
#define EDITAWARDDIALOG_H

#include <QDialog>
#include "Award.h"
#include <QStringList>

namespace Ui {
class EditAwardDialog;
}

class EditAwardDialog : public QDialog
{
    Q_OBJECT

public:
    explicit EditAwardDialog(const Award &award, QWidget *parent = nullptr);
    ~EditAwardDialog() override;

    Award getAward() const;
    void fillSportComboBox(const QStringList& sports);
    void fillLevelComboBox(const QStringList& levels);

private slots:
    void on_buttonBox_accepted();

private:
    Ui::EditAwardDialog *ui;
    Award m_award;
};

#endif
