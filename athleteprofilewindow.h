#ifndef ATHLETEPROFILEWINDOW_H
#define ATHLETEPROFILEWINDOW_H

#include <QMainWindow>
#include "Athlete.h" // Assuming you have an Athlete class

QT_BEGIN_NAMESPACE
namespace Ui { class AthleteProfileWindow; }
QT_END_NAMESPACE

class AthleteProfileWindow : public QMainWindow
{
    Q_OBJECT

public:
    AthleteProfileWindow(Athlete athlete, QWidget *parent = nullptr);
    ~AthleteProfileWindow() override;

private:
    Ui::AthleteProfileWindow *ui;
    Athlete m_athlete;

    void populateUI();
};

#endif // ATHLETEPROFILEWINDOW_H
