#include "MainWindow.h"
#include "ui_mainwindow.h"
#include "AddAwardDialog.h"
#include "EditAwardDialog.h"
#include "AwardDetailsDialog.h"
#include "AthleteProfileWindow.h"
#include "DatabaseManager.h"
#include <QMessageBox>
#include <QFileDialog>
#include <QTextStream>
#include <QVariant>
#include <QDialogButtonBox>
#include <QDebug>
#include "LoginWindow.h"
#include "AwardTableModel.h"
#include "ReportGenerator.h"// Include AwardTableModel

MainWindow::MainWindow(QWidget *parent, Athlete loggedInAthlete)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_loggedInAthlete(loggedInAthlete)
    , m_dataManager(DatabaseManager::getInstance())
{
    ui->setupUi(this);

    ui->userNameLabel->setText(m_loggedInAthlete.getFirstName() + " " + m_loggedInAthlete.getLastName());

    m_awardTableModel = new AwardTableModel(this); // Initialize the model
    ui->awardsTableView->setModel(m_awardTableModel);

    // Connect signals and slots
    connect(ui->addAwardButton, &QPushButton::clicked, this, &MainWindow::on_addAwardButton_clicked);
    connect(ui->editAwardButton, &QPushButton::clicked, this, &MainWindow::on_editAwardButton_clicked);
    connect(ui->deleteAwardButton, &QPushButton::clicked, this, &MainWindow::on_deleteAwardButton_clicked);
    connect(ui->searchLineEdit, &QLineEdit::textChanged, this, &MainWindow::on_searchLineEdit_textChanged);

    connect(ui->sportFilterComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &MainWindow::on_sportFilterComboBox_currentIndexChanged);
    connect(ui->levelFilterComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &MainWindow::on_levelFilterComboBox_currentIndexChanged);

    connect(ui->reportButton, &QPushButton::clicked, this, &MainWindow::on_reportButton_clicked);
    connect(ui->editProfileButton, &QPushButton::clicked, this, &MainWindow::on_editProfileButton_clicked);
    connect(ui->infoButton, &QPushButton::clicked, this, &MainWindow::on_infoButton_clicked);
    connect(ui->logoutButton, &QPushButton::clicked, this, &MainWindow::on_logoutButton_clicked);

    connect(ui->awardsTableView, &QTableView::doubleClicked, this, &MainWindow::on_awardsTableView_doubleClicked);

    // Initialize the combo boxes
    fillSportComboBox();
    fillLevelComboBox();

    updateAwardList();
}

MainWindow::~MainWindow()
{
    delete ui;
    delete m_awardTableModel;
}

void MainWindow::on_addAwardButton_clicked()
{
    AddAwardDialog dialog(this, m_loggedInAthlete.getId());

    QStringList sports = {"Легкая атлетика", "Плавание", "Велоспорт", "Командный спорт", "Футбол", "Баскетбол", "Волейбол", "Теннис", "Бокс", "Другое"};
    QStringList levels = {"Местный", "Региональный", "Национальный", "Международный", "Олимпийский"};
    dialog.fillSportComboBox(sports);
    dialog.fillLevelComboBox(levels);

    if (dialog.exec() == QDialog::Accepted) {
        Award newAward = dialog.getAward();
        DatabaseManager& dbManager = DatabaseManager::getInstance();

        QSqlQuery query(dbManager.getDatabase());

        query.prepare("INSERT INTO awards (name, date, location, sport, discipline, level, place, document, athlete_id) "
                      "VALUES (:name, :date, :location, :sport, :discipline, :level, :place, :document, :athlete_id)");
        query.bindValue(":name", newAward.getName());
        query.bindValue(":date", newAward.getDate());
        query.bindValue(":location", newAward.getLocation());
        query.bindValue(":sport", static_cast<int>(newAward.getSport()));
        query.bindValue(":discipline", newAward.getDiscipline());
        query.bindValue(":level", static_cast<int>(newAward.getLevel()));
        query.bindValue(":place", newAward.getPlace());
        query.bindValue(":document", newAward.getDocument());
        query.bindValue(":athlete_id", m_loggedInAthlete.getId());

        if (!query.exec()) {
            qDebug() << "Error adding award:" << query.lastError().text();
        }
        updateAwardList();
    }
}

void MainWindow::on_editAwardButton_clicked()
{
    QModelIndex index = ui->awardsTableView->currentIndex();

    if (!index.isValid()) {
        QMessageBox::warning(this, "Предупреждение", "Пожалуйста, выберите награду для редактирования.");
        return;
    }

    // Get the ID directly from the model
    int awardId = m_awardTableModel->data(index, AwardTableModel::IdRole).toInt(); // Get the ID

    // Get the Award object using the ID
    Award selectedAward = m_awards.at(index.row());

    EditAwardDialog dialog(selectedAward, this);

    QStringList sports = {"Легкая атлетика", "Плавание", "Велоспорт", "Командный спорт", "Футбол", "Баскетбол", "Волейбол", "Теннис", "Бокс", "Другое"};
    QStringList levels = {"Местный", "Региональный", "Национальный", "Международный", "Олимпийский"};
    dialog.fillSportComboBox(sports);
    dialog.fillLevelComboBox(levels);

    if (dialog.exec() == QDialog::Accepted) {
        Award editedAward = dialog.getAward();

        DatabaseManager& dbManager = DatabaseManager::getInstance();
        dbManager.updateAward(editedAward);
        updateAwardList();
    }
}

void MainWindow::on_deleteAwardButton_clicked()
{
    QModelIndex index = ui->awardsTableView->currentIndex();

    if (!index.isValid()) {
        QMessageBox::warning(this, "Предупреждение", "Пожалуйста, выберите награду для удаления.");
        return;
    }

    // Get the award ID directly from the model
    int awardId = m_awardTableModel->data(index, AwardTableModel::IdRole).toInt(); // Correctly get the ID

    QMessageBox::StandardButton reply = QMessageBox::question(this, "Удаление награды",
                                                              "Вы уверены, что хотите удалить эту награду?",
                                                              QMessageBox::Yes | QMessageBox::No);
    if (reply == QMessageBox::Yes) {
        DatabaseManager& dbManager = DatabaseManager::getInstance();
        // Remove the award using the ID
        Award awardToDelete;
        awardToDelete.setId(awardId);
        dbManager.removeAward(awardToDelete);
        updateAwardList();
    }
}

void MainWindow::on_searchLineEdit_textChanged(const QString &arg1)
{
    qDebug() << "Search text changed to: " << arg1;
    m_awardTableModel->searchAwards(arg1);
}


void MainWindow::updateAwardList()
{
    if (m_loggedInAthlete.getId() <= 0) {
        m_awardTableModel->setAwards(QList<Award>());
        return;
    }

    DatabaseManager& dbManager = DatabaseManager::getInstance();
    QList<Award> awards = dbManager.getAllAwards(m_loggedInAthlete.getId());
    m_awards = awards; // Update local awards list
    m_awardTableModel->setAwards(awards); // Update the model with the new data
}

void MainWindow::on_awardListWidget_itemDoubleClicked(QListWidgetItem *item)
{

}

void MainWindow::on_awardsTableView_doubleClicked(const QModelIndex &index)
{
    if (index.isValid()) {
        Award award = m_awardTableModel->data(index, AwardTableModel::IdRole).value<Award>();
        AwardDetailsDialog detailsDialog(award, this);
        detailsDialog.exec();
    }
}

void MainWindow::on_reportButton_clicked()
{
    QString filePath = QFileDialog::getSaveFileName(this, "Сохранить отчет", "", "PDF Files (*.pdf)");
    if (!filePath.isEmpty()) {
        if (m_reportGenerator->generateReport(m_awards, filePath)) {
            QMessageBox::information(this, "Отчет создан", "Отчет успешно сохранен в " + filePath);
        } else {
            QMessageBox::critical(this, "Ошибка", "Не удалось создать отчет.");
        }
    }
}

QString MainWindow::sportTypeToString(SportType type) {
    switch (type) {
    case SportType::Athletics: return "Легкая атлетика";
    case SportType::Swimming: return "Плавание";
    case SportType::Cycling: return "Велоспорт";
    case SportType::TeamSport: return "Командный спорт";
    case SportType::Football: return "Футбол";
    case SportType::Basketball: return "Баскетбол";
    case SportType::Volleyball: return "Волейбол";
    case SportType::Tennis: return "Теннис";
    case SportType::Boxing: return "Бокс";
    case SportType::Other: return "Другое";
    default: return "Неизвестно";
    }
}

QString MainWindow::competitionLevelToString(CompetitionLevel level) {
    switch (level) {
    case CompetitionLevel::Local: return "Местный";
    case CompetitionLevel::Regional: return "Региональный";
    case CompetitionLevel::National: return "Национальный";
    case CompetitionLevel::International: return "Международный";
    case CompetitionLevel::Olympic: return "Олимпийский";
    default: return "Неизвестно";
    }
}

void MainWindow::on_infoButton_clicked()
{
    AthleteProfileWindow *profileWindow = new AthleteProfileWindow(m_loggedInAthlete, this);
    profileWindow->show();
}

void MainWindow::on_editProfileButton_clicked()
{
    AthleteProfileWindow *profileWindow = new AthleteProfileWindow(m_loggedInAthlete, this);
    profileWindow->show();
}

void MainWindow::on_logoutButton_clicked()
{
    this->close();

    LoginWindow *loginWindow = new LoginWindow();
    loginWindow->show();
}
void MainWindow::on_sportFilterComboBox_currentIndexChanged(int index)
{
    QString sport = ui->sportFilterComboBox->itemText(index);
    m_awardTableModel->filterAwards(sport, ui->levelFilterComboBox->currentText());
}

void MainWindow::on_levelFilterComboBox_currentIndexChanged(int index)
{
    QString level = ui->levelFilterComboBox->itemText(index);
    m_awardTableModel->filterAwards(ui->sportFilterComboBox->currentText(), level);
}
void MainWindow::fillSportComboBox()
{
    QStringList sports = {"", "Легкая атлетика", "Плавание", "Велоспорт", "Командный спорт", "Футбол", "Баскетбол", "Волейбол", "Теннис", "Бокс", "Другое"};
    ui->sportFilterComboBox->addItems(sports);
    qDebug() << "Sport ComboBox count: " << ui->sportFilterComboBox->count(); // Добавляем отладочный вывод
}

void MainWindow::fillLevelComboBox()
{
    QStringList levels = {"", "Местный", "Региональный", "Национальный", "Международный", "Олимпийский"};
    ui->levelFilterComboBox->addItems(levels);
    qDebug() << "Level ComboBox count: " << ui->levelFilterComboBox->count(); // Добавляем отладочный вывод
}
