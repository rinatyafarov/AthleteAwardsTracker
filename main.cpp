#include "mainwindow.h"
#include "loginwindow.h"
#include "databasemanager.h"
#include <QApplication>
#include <QMessageBox>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // Initialize database connection
    DatabaseManager& dbManager = DatabaseManager::getInstance();
    bool connected = dbManager.connectToDatabase("athlete_awards.db");

    if (!connected) {
        QMessageBox::critical(nullptr, "Database Error", "Failed to connect to the database. The application will exit.");
        return 1; // Exit the application
    }

    LoginWindow loginWindow;
    QObject::connect(&loginWindow, &LoginWindow::loginSuccessful,
                     [&](const QString& login){
                         // Show the main window after successful login
                         MainWindow w;
                         w.show();
                     });

    loginWindow.exec(); // Use exec() for modal dialogs

    return a.exec();
}
