#ifndef ATHLETEPROFILEWINDOW_H
#define ATHLETEPROFILEWINDOW_H

#include <QDialog>
#include "Athlete.h"

namespace Ui {
class AthleteProfileWindow;
}

class AthleteProfileWindow : public QDialog
{
    Q_OBJECT

public:
    explicit AthleteProfileWindow(Athlete athlete, QWidget *parent = nullptr);
    ~AthleteProfileWindow() override;

private slots:
    void on_saveButton_clicked();

private:
    Ui::AthleteProfileWindow *ui;
    Athlete m_athlete;
};

#endif
