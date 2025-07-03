#include "mainwindow.h"
#include "loginwindow.h"
#include "databasemanager.h"
#include <QApplication>
#include <QMessageBox>
#include <QDebug>  // Include QDebug
#include "Athlete.h"

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

    // Connect the loginSuccessful signal to a lambda function that shows the main window
    QObject::connect(&loginWindow, &LoginWindow::loginSuccessful,
                     [&](Athlete loggedInAthlete) { // Capture Athlete object
                         qDebug() << "Login successful for user: " << loggedInAthlete.getLogin();
                         MainWindow *w = new MainWindow(nullptr, loggedInAthlete); // Pass the Athlete object
                         w->show();
                     });

    int result = loginWindow.exec(); // Use exec() for modal dialogs
    qDebug() << "Login window exited with code: " << result;  // Add debug message

    return a.exec();
}
