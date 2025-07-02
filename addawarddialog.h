#ifndef ADDAWARDDIALOG_H
#define ADDAWARDDIALOG_H

#include <QDialog>
#include "Award.h"

namespace Ui {
class AddAwardDialog;
}

class AddAwardDialog : public QDialog
{
    Q_OBJECT

public:
    AddAwardDialog(QWidget *parent = nullptr);
    ~AddAwardDialog() override;

    Award getAward() const;

    // Public methods to fill combo boxes
    void fillSportComboBox(const QStringList& sports);
    void fillLevelComboBox(const QStringList& levels);

private slots:
    void on_buttonBox_accepted();

private:
    Ui::AddAwardDialog *ui;
};

#endif // ADDAWARDDIALOG_H
