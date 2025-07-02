#include "MainWindow.h"
#include "ui_mainwindow.h"
#include "AddAwardDialog.h"
#include "EditAwardDialog.h"
#include "AwardDetailsDialog.h"// Include AwardDetailsDialog
#include "AthleteProfileWindow.h"
#include <QMessageBox>
#include <QFileDialog>
#include <QTextStream>
//#include <QTextCodec>
#include <QVariant> // Для QVariant::fromValue
#include <QDialogButtonBox>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    updateAwardList();
    connect(&m_dataManager, &DataManager::awardsChanged, this, &MainWindow::updateAwardList);

    // Подключение сигнала doubleClicked для отображения деталей награды
    connect(ui->awardListWidget, &QListWidget::itemDoubleClicked, this, &MainWindow::on_awardListWidget_itemDoubleClicked);
    connect(ui->athleteListWidget, &QListWidget::itemDoubleClicked, this, &MainWindow::on_athleteDoubleClicked);
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
        m_dataManager.addAward(newAward);
        updateAwardList();
    }
}


void MainWindow::on_editAwardButton_clicked()
{
    QListWidgetItem *selectedItem = ui->awardListWidget->currentItem();
    if (!selectedItem) {
        QMessageBox::warning(this, "Предупреждение", "Пожалуйста, выберите награду для редактирования.");
        return;
    }

    int index = ui->awardListWidget->currentRow();
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
            m_dataManager.updateAward(editedAward);
            updateAwardList();
        }
    }
}


void MainWindow::on_deleteAwardButton_clicked()
{
    QListWidgetItem *selectedItem = ui->awardListWidget->currentItem();
    if (!selectedItem) {
        QMessageBox::warning(this, "Предупреждение", "Пожалуйста, выберите награду для удаления.");
        return;
    }

    int index = ui->awardListWidget->currentRow();
    if (index >= 0 && index < m_awards.size()) {
        Award awardToDelete = m_awards[index];
        QMessageBox::StandardButton reply = QMessageBox::question(this, "Удаление награды",
                                                                  "Вы уверены, что хотите удалить эту награду?",
                                                                  QMessageBox::Yes | QMessageBox::No);
        if (reply == QMessageBox::Yes) {
            m_dataManager.removeAward(awardToDelete);
            updateAwardList();
        }
    }
}


void MainWindow::on_searchLineEdit_textChanged(const QString &arg1)
{
    QList<Award> searchResults = m_dataManager.searchAwards(arg1);
    ui->awardListWidget->clear();
    for (const Award& award : searchResults) {
        QListWidgetItem *item = new QListWidgetItem(award.getName());
        // Сохраняем весь объект Award в data для последующего использования
        item->setData(Qt::UserRole, QVariant::fromValue(award));
        ui->awardListWidget->addItem(item);
    }
}

void MainWindow::on_filterButton_clicked()
{
    //TODO: Реализовать фильтрацию
    QMessageBox::information(this, "Фильтр", "Функция фильтрации пока не реализована.");
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

// Слот для обработки двойного клика по элементу списка
void MainWindow::on_awardListWidget_itemDoubleClicked(QListWidgetItem *item)
{
    // Получаем объект Award из данных элемента списка
    Award selectedAward = item->data(Qt::UserRole).value<Award>();
    AwardDetailsDialog dialog(selectedAward, this);
    dialog.exec(); // Отображаем диалог деталей
}

void MainWindow::on_athleteDoubleClicked(QListWidgetItem *item) {
    Athlete athlete = item->data(Qt::UserRole).value<Athlete>();
    AthleteProfileWindow *profileWindow = new AthleteProfileWindow(athlete, this); // Создаем новое окно профиля
    profileWindow->show(); // Показываем окно профиля
}

void MainWindow::updateAwardList()
{
    m_awards = m_dataManager.getAwards();
    ui->awardListWidget->clear();
    for (const Award& award : m_awards) {
        QListWidgetItem *item = new QListWidgetItem(award.getName());
        // Сохраняем весь объект Award в data для последующего использования
        item->setData(Qt::UserRole, QVariant::fromValue(award));
        ui->awardListWidget->addItem(item);
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
