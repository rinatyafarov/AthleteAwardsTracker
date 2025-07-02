#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QList>
#include "Award.h"  // Include the Award class
#include "DataManager.h"
#include "QListWidgetItem" // Include the DataManager class

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

private slots:
    void on_addAwardButton_clicked();
    void on_editAwardButton_clicked();
    void on_deleteAwardButton_clicked();
    void on_searchLineEdit_textChanged(const QString &arg1);
    void on_filterButton_clicked();
    void on_reportButton_clicked();
    void updateAwardList();
    void on_awardListWidget_itemDoubleClicked(QListWidgetItem *item); // Correct declaration
    void on_athleteDoubleClicked(QListWidgetItem *item);

private:
    Ui::MainWindow *ui;
    DataManager& m_dataManager = DataManager::getInstance();
    QList<Award> m_awards;

    // Declare helper functions for enum to string conversion
    QString sportTypeToString(SportType type);
    QString competitionLevelToString(CompetitionLevel level);
};
#endif // MAINWINDOW_H
