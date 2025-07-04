#ifndef AWARDDETAILSDIALOG_H
#define AWARDDETAILSDIALOG_H

#include <QDialog>
#include "Award.h"

namespace Ui {
class AwardDetailsDialog;
}

class AwardDetailsDialog : public QDialog
{
    Q_OBJECT

public:
    AwardDetailsDialog(const Award& award, QWidget *parent = nullptr);
    ~AwardDetailsDialog() override;

private:
    Ui::AwardDetailsDialog *ui;
    Award m_award;
};

#endif
