#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QList>
#include "Award.h"
#include "Athlete.h"
#include "databasemanager.h"
#include <QListWidgetItem> // Include QListWidgetItem
#include <QString>  // Include QString
#include "AwardTableModel.h" // Include AwardTableModel

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr, Athlete loggedInAthlete = Athlete());
    ~MainWindow() override;

    // Declare helper functions for enum to string conversion and make them public
    static QString sportTypeToString(SportType type);
    static QString competitionLevelToString(CompetitionLevel level);

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
    void on_awardsTableView_doubleClicked(const QModelIndex &index);

private:
    Ui::MainWindow *ui;
    DatabaseManager& m_dataManager; // Initialize in constructor
    Athlete m_loggedInAthlete;
    QList<Award> m_awards;
    int m_loggedInAthleteId; // Store the athlete's ID

    AwardTableModel *m_awardTableModel; // Add member

};

#endif // MAINWINDOW_H
