#include "mainwindow.h"
#include "loginwindow.h"
#include "databasemanager.h"
#include <QApplication>
#include <QMessageBox>
#include <QDebug>
#include "Athlete.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);


    DatabaseManager& dbManager = DatabaseManager::getInstance();
    bool connected = dbManager.connectToDatabase("athlete_awards.db");

    if (!connected) {
        QMessageBox::critical(nullptr, "Database Error", "Failed to connect to the database. The application will exit.");
        return 1;
    }

    LoginWindow loginWindow;


    QObject::connect(&loginWindow, &LoginWindow::loginSuccessful,
                     [&](Athlete loggedInAthlete) {
                         qDebug() << "Login successful for user: " << loggedInAthlete.getLogin();
                         MainWindow *w = new MainWindow(nullptr, loggedInAthlete);
                         w->show();
                     });

    int result = loginWindow.exec();
    qDebug() << "Login window exited with code: " << result;

    return a.exec();
}
