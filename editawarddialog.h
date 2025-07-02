#ifndef EDITAWARDDIALOG_H
#define EDITAWARDDIALOG_H

#include <QDialog>
#include "Award.h"

namespace Ui {
class EditAwardDialog;
}

class EditAwardDialog : public QDialog
{
    Q_OBJECT

public:
    EditAwardDialog(const Award &award, QWidget *parent = nullptr);
    ~EditAwardDialog() override;

    Award getAward() const;

    // Public methods to fill combo boxes
    void fillSportComboBox(const QStringList& sports);
    void fillLevelComboBox(const QStringList& levels);

private slots:
    void on_buttonBox_accepted();

private:
    Ui::EditAwardDialog *ui;
    Award m_award;
};

#endif // EDITAWARDDIALOG_H
