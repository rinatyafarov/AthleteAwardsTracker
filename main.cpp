#include "mainwindow.h"
#include "loginwindow.h"
#include "databasemanager.h"
#include <QApplication>
#include <QMessageBox>
#include <QDebug>  // Include QDebug

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
                     [&]() { // No argument needed
                         qDebug() << "Login successful, showing main window";  // Add debug message
                         MainWindow *w = new MainWindow(); // Create MainWindow on the heap
                         w->show();
                     });

    int result = loginWindow.exec(); // Use exec() for modal dialogs
    qDebug() << "Login window exited with code: " << result;  // Add debug message

    return a.exec();
}
