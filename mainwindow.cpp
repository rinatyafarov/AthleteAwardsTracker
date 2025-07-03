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

MainWindow::MainWindow(QWidget *parent, Athlete loggedInAthlete)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_loggedInAthlete(loggedInAthlete)
    , m_dataManager(DatabaseManager::getInstance())
{
    ui->setupUi(this);

    ui->userNameLabel->setText(m_loggedInAthlete.getFirstName() + " " + m_loggedInAthlete.getLastName());

    connect(ui->addAwardButton, &QPushButton::clicked, this, &MainWindow::on_addAwardButton_clicked);
    connect(ui->editAwardButton, &QPushButton::clicked, this, &MainWindow::on_editAwardButton_clicked);
    connect(ui->deleteAwardButton, &QPushButton::clicked, this, &MainWindow::on_deleteAwardButton_clicked);
    connect(ui->searchLineEdit, &QLineEdit::textChanged, this, &MainWindow::on_searchLineEdit_textChanged);
    connect(ui->filterButton, &QPushButton::clicked, this, &MainWindow::on_filterButton_clicked);
    connect(ui->reportButton, &QPushButton::clicked, this, &MainWindow::on_reportButton_clicked);
    connect(ui->editProfileButton, &QPushButton::clicked, this, &MainWindow::on_editProfileButton_clicked);
    connect(ui->infoButton, &QPushButton::clicked, this, &MainWindow::on_infoButton_clicked);
    connect(ui->awardsListWidget, &QListWidget::itemDoubleClicked, this, &MainWindow::on_awardListWidget_itemDoubleClicked);
    connect(ui->logoutButton, &QPushButton::clicked, this, &MainWindow::on_logoutButton_clicked);

    updateAwardList();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_addAwardButton_clicked()
{
    AddAwardDialog dialog(this, m_loggedInAthlete.getId()); // Pass athleteId

    QStringList sports = {"Легкая атлетика", "Плавание", "Велоспорт", "Командный спорт", "Футбол", "Баскетбол", "Волейбол", "Теннис", "Бокс", "Другое"};
    QStringList levels = {"Местный", "Региональный", "Национальный", "Международный", "Олимпийский"};
    dialog.fillSportComboBox(sports);
    dialog.fillLevelComboBox(levels);

    if (dialog.exec() == QDialog::Accepted) {
        Award newAward = dialog.getAward();
        DatabaseManager& dbManager = DatabaseManager::getInstance();

        QSqlQuery query(dbManager.getDatabase()); // Use getDatabase()

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
    QListWidgetItem *selectedItem = ui->awardsListWidget->currentItem();
    if (!selectedItem) {
        QMessageBox::warning(this, "Предупреждение", "Пожалуйста, выберите награду для редактирования.");
        return;
    }

    int index = ui->awardsListWidget->currentRow();
    if (index >= 0 && index < m_awards.size()) {
        Award selectedAward = m_awards[index];
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
}

void MainWindow::on_deleteAwardButton_clicked()
{
    QListWidgetItem *selectedItem = ui->awardsListWidget->currentItem();
    if (!selectedItem) {
        QMessageBox::warning(this, "Предупреждение", "Пожалуйста, выберите награду для удаления.");
        return;
    }

    int index = ui->awardsListWidget->currentRow();
    if (index >= 0 && index < m_awards.size()) {
        Award awardToDelete = m_awards[index];
        QMessageBox::StandardButton reply = QMessageBox::question(this, "Удаление награды",
                                                                  "Вы уверены, что хотите удалить эту награду?",
                                                                  QMessageBox::Yes | QMessageBox::No);
        if (reply == QMessageBox::Yes) {
            DatabaseManager& dbManager = DatabaseManager::getInstance();
            dbManager.removeAward(awardToDelete);
            updateAwardList();
        }
    }
}

void MainWindow::on_searchLineEdit_textChanged(const QString &arg1)
{
    QMessageBox::information(this, "Поиск", "Функция поиска пока не реализована.");
}

void MainWindow::on_filterButton_clicked()
{
    QMessageBox::information(this, "Фильтр", "Функция фильтрации пока не реализована.");
}

void MainWindow::updateAwardList()
{
    if (m_loggedInAthlete.getId() <= 0) {
        ui->awardsListWidget->clear();
        return;
    }

    DatabaseManager& dbManager = DatabaseManager::getInstance();
    QList<Award> awards = dbManager.getAllAwards(m_loggedInAthlete.getId());
    ui->awardsListWidget->clear();
    for (const Award& award : awards) {
        QListWidgetItem *item = new QListWidgetItem(award.getName());
        item->setData(Qt::UserRole, QVariant::fromValue(award));
        ui->awardsListWidget->addItem(item);
    }
    m_awards = awards;
}

void MainWindow::on_awardListWidget_itemDoubleClicked(QListWidgetItem *item)
{
    if (item) {
        Award award = item->data(Qt::UserRole).value<Award>();
        AwardDetailsDialog detailsDialog(award, this);
        detailsDialog.exec();
    }
}

void MainWindow::on_reportButton_clicked()
{
    QString fileName = QFileDialog::getSaveFileName(this, "Сохранить отчет", "", "CSV Файлы (*.csv);;Все файлы (*)");
    if (!fileName.isEmpty()) {
        QFile file(fileName);
        if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
            QTextStream stream(&file);
            stream.setEncoding(QStringConverter::Utf8);

            stream << "Название,Дата,Место проведения,Вид спорта,Дисциплина,Уровень,Занятое место,Подтверждающий документ\n";

            for (const Award& award : m_awards) {
                stream << award.getName() << ","
                       << award.getDate().toString("yyyy-MM-dd") << ","
                       << award.getLocation() << ","
                       << sportTypeToString(award.getSport()) << ","
                       << award.getDiscipline() << ","
                       << competitionLevelToString(award.getLevel()) << ","
                       << award.getPlace() << ","
                       << award.getDocument() << "\n";
            }

            file.close();
            QMessageBox::information(this, "Отчет сохранен", "Отчет успешно сохранен в: " + fileName);
        } else {
            QMessageBox::critical(this, "Ошибка", "Не удалось сохранить отчет в: " + fileName);
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
    QMessageBox::information(this, "Редактировать профиль", "Функция редактирования профиля пока не реализована.");
}

void MainWindow::on_logoutButton_clicked()
{
    this->close();

    LoginWindow *loginWindow = new LoginWindow();
    loginWindow->show();
}
