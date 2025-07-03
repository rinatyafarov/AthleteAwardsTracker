#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QList>
#include "Award.h"  // Include the Award class
#include "Athlete.h"  // Include the Athlete class
#include "DataManager.h"
#include <QListWidgetItem> // Include QListWidgetItem
#include <QString>  // Include QString

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr, Athlete loggedInAthlete = Athlete());
    ~MainWindow() override;

private slots:
    void on_addAwardButton_clicked();
    void on_editAwardButton_clicked();
    void on_deleteAwardButton_clicked();
    void on_searchLineEdit_textChanged(const QString &arg1);
    void on_filterButton_clicked();
    void on_reportButton_clicked();
    void updateAwardList();
    void on_awardListWidget_itemDoubleClicked(QListWidgetItem *item);
    void on_editProfileButton_clicked();
    void on_infoButton_clicked();
    void on_logoutButton_clicked();

private:
    Ui::MainWindow *ui;
    DataManager& m_dataManager; // Initialize in constructor
    Athlete m_loggedInAthlete;
    QList<Award> m_awards;

    // Declare helper functions for enum to string conversion
    static QString sportTypeToString(SportType type);
    static QString competitionLevelToString(CompetitionLevel level);
};
#endif // MAINWINDOW_H
