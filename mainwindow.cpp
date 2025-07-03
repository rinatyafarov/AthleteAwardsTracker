#include "MainWindow.h"
#include "ui_mainwindow.h"
#include "AddAwardDialog.h"
#include "EditAwardDialog.h"
#include "AwardDetailsDialog.h" // Include AwardDetailsDialog
#include "AthleteProfileWindow.h"
#include "DatabaseManager.h"
#include <QMessageBox>
#include <QFileDialog>
#include <QTextStream>
#include <QVariant> // Для QVariant::fromValue
#include <QDialogButtonBox>
#include <QDebug>
#include "LoginWindow.h"

MainWindow::MainWindow(QWidget *parent, Athlete loggedInAthlete)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_loggedInAthlete(loggedInAthlete)
    , m_dataManager(DataManager::getInstance())  // Correctly initialize m_dataManager
{
    ui->setupUi(this);

    //  Отображение информации об авторизованном пользователе
    ui->userNameLabel->setText(m_loggedInAthlete.getFirstName() + " " + m_loggedInAthlete.getLastName());

    // Подключение сигналов к слотам
    connect(ui->addAwardButton, &QPushButton::clicked, this, &MainWindow::on_addAwardButton_clicked);
    connect(ui->editAwardButton, &QPushButton::clicked, this, &MainWindow::on_editAwardButton_clicked);
    connect(ui->deleteAwardButton, &QPushButton::clicked, this, &MainWindow::on_deleteAwardButton_clicked);
    connect(ui->searchLineEdit, &QLineEdit::textChanged, this, &MainWindow::on_searchLineEdit_textChanged);
    connect(ui->filterButton, &QPushButton::clicked, this, &MainWindow::on_filterButton_clicked);
    connect(ui->reportButton, &QPushButton::clicked, this, &MainWindow::on_reportButton_clicked); // Connect Report Button
    connect(ui->editProfileButton, &QPushButton::clicked, this, &MainWindow::on_editProfileButton_clicked); // Connect Edit Profile Button
    connect(ui->infoButton, &QPushButton::clicked, this, &MainWindow::on_infoButton_clicked);         // Connect Info Button
    connect(ui->awardsListWidget, &QListWidget::itemDoubleClicked, this, &MainWindow::on_awardListWidget_itemDoubleClicked);
    connect(ui->logoutButton, &QPushButton::clicked, this, &MainWindow::on_logoutButton_clicked);

    //  Обновление списка наград
    updateAwardList();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_addAwardButton_clicked()
{
    AddAwardDialog dialog(this);

    // Fill combo boxes
    QStringList sports = {"Легкая атлетика", "Плавание", "Велоспорт", "Командный спорт", "Футбол", "Баскетбол", "Волейбол", "Теннис", "Бокс", "Другое"};
    QStringList levels = {"Местный", "Региональный", "Национальный", "Международный", "Олимпийский"};
    dialog.fillSportComboBox(sports);
    dialog.fillLevelComboBox(levels);

    if (dialog.exec() == QDialog::Accepted) {
        Award newAward = dialog.getAward();
        DatabaseManager& dbManager = DatabaseManager::getInstance(); // Получаем экземпляр DatabaseManager
        dbManager.addAward(newAward);
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

        // Fill combo boxes
        QStringList sports = {"Легкая атлетика", "Плавание", "Велоспорт", "Командный спорт", "Футбол", "Баскетбол", "Волейбол", "Теннис", "Бокс", "Другое"};
        QStringList levels = {"Местный", "Региональный", "Национальный", "Международный", "Олимпийский"};
        dialog.fillSportComboBox(sports);
        dialog.fillLevelComboBox(levels);

        if (dialog.exec() == QDialog::Accepted) {
            Award editedAward = dialog.getAward();
            DatabaseManager& dbManager = DatabaseManager::getInstance(); // Получаем экземпляр DatabaseManager
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
            DatabaseManager& dbManager = DatabaseManager::getInstance(); // Получаем экземпляр DatabaseManager
            dbManager.removeAward(awardToDelete);
            updateAwardList();
        }
    }
}

void MainWindow::on_searchLineEdit_textChanged(const QString &arg1)
{
    //  реализовать поиск
    //QList searchResults = m_dataManager.searchAwards(arg1);  Сейчас не работает, так как нет DataManager
    // ui->awardsListWidget->clear();

    //for (const Award& award : searchResults) {
    //QListWidgetItem *item = new QListWidgetItem(award.getName());
    // item->setData(Qt::UserRole, QVariant::fromValue(award));  // Сохраняем весь объект Award в data для последующего использования
    //ui->awardsListWidget->addItem(item);
    //}
    QMessageBox::information(this, "Поиск", "Функция поиска пока не реализована.");
}

void MainWindow::on_filterButton_clicked()
{
    //TODO: Реализовать фильтрацию
    QMessageBox::information(this, "Фильтр", "Функция фильтрации пока не реализована.");
}

void MainWindow::updateAwardList()
{
    DatabaseManager& dbManager = DatabaseManager::getInstance();
    QList awards = dbManager.getAllAwards();
    ui->awardsListWidget->clear();
    for (const Award& award : awards) {
        QListWidgetItem *item = new QListWidgetItem(award.getName());
        item->setData(Qt::UserRole, QVariant::fromValue(award));  // Сохраняем весь объект Award в data для последующего использования
        ui->awardsListWidget->addItem(item);
    }
    m_awards = awards;
}

void MainWindow::on_awardListWidget_itemDoubleClicked(QListWidgetItem *item)
{
    if (item) {
        Award award = item->data(Qt::UserRole).value<Award>(); // Corrected line
        AwardDetailsDialog detailsDialog(award, this);
        detailsDialog.exec(); // Отображаем диалог деталей
    }
}

void MainWindow::on_reportButton_clicked()
{
    QString fileName = QFileDialog::getSaveFileName(this, "Сохранить отчет", "", "CSV Файлы (*.csv);;Все файлы (*)");
    if (!fileName.isEmpty()) {
        QFile file(fileName);
        if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
            QTextStream stream(&file);
            stream.setEncoding(QStringConverter::Utf8); // Используем UTF-8 кодировку

            // Записываем заголовок CSV
            stream << "Название,Дата,Место проведения,Вид спорта,Дисциплина,Уровень,Занятое место,Подтверждающий документ\n";

            // Записываем данные наград
            for (const Award& award : m_awards) {
                stream << award.getName() << ","
                       << award.getDate().toString("yyyy-MM-dd") << ","
                       << award.getLocation() << ","
                       << sportTypeToString(award.getSport()) << "," // Используем преобразованные строки
                       << award.getDiscipline() << ","
                       << competitionLevelToString(award.getLevel()) << "," // Используем преобразованные строки
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
    // Implement info functionality
    AthleteProfileWindow *profileWindow = new AthleteProfileWindow(m_loggedInAthlete, this); // Create new profile window
    profileWindow->show(); // Show profile window
}

void MainWindow::on_editProfileButton_clicked()
{
    // Implement edit profile functionality
    QMessageBox::information(this, "Редактировать профиль", "Функция редактирования профиля пока не реализована.");
}
void MainWindow::on_logoutButton_clicked()
{
    LoginWindow *loginWindow = new LoginWindow(); // Create a new LoginWindow
    loginWindow->show(); // Show the login window

    this->close(); // Close the main window
}
