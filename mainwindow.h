#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QList>
#include "Award.h"
#include "Athlete.h"
#include "databasemanager.h"
#include <QListWidgetItem>
#include <QString>
#include "AwardTableModel.h"
#include "ReportGenerator.h"
#include <QFileDialog>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr, Athlete loggedInAthlete = Athlete());
    ~MainWindow() override;


    static QString sportTypeToString(SportType type);
    static QString competitionLevelToString(CompetitionLevel level);

private slots:
    void on_addAwardButton_clicked();
    void on_editAwardButton_clicked();
    void on_deleteAwardButton_clicked();
    void on_searchLineEdit_textChanged(const QString &arg1);
    void on_reportButton_clicked();
    void updateAwardList();
    void on_awardListWidget_itemDoubleClicked(QListWidgetItem *item);
    void on_editProfileButton_clicked();
    void on_infoButton_clicked();
    void on_logoutButton_clicked();
    void on_awardsTableView_doubleClicked(const QModelIndex &index);
    void on_sportFilterComboBox_currentIndexChanged(int index);
    void on_levelFilterComboBox_currentIndexChanged(int index);

private:
    Ui::MainWindow *ui;
    DatabaseManager& m_dataManager;
    Athlete m_loggedInAthlete;
    QList<Award> m_awards;
    int m_loggedInAthleteId;

    AwardTableModel *m_awardTableModel;
    ReportGenerator *m_reportGenerator = new ReportGenerator(this);

    void fillSportComboBox();
    void fillLevelComboBox();
};

#endif
