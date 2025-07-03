#ifndef LOGINWINDOW_H
#define LOGINWINDOW_H

#include <QDialog>
#include "Athlete.h"  // Include Athlete

namespace Ui {
class LoginWindow;
}

class LoginWindow : public QDialog
{
    Q_OBJECT

public:
    LoginWindow(QWidget *parent = nullptr);
    ~LoginWindow() override;

signals:
    void loginSuccessful(Athlete athlete);  // Changed Signal

private slots:
    void on_loginButton_clicked();
    void on_registerButton_clicked();

private:
    Ui::LoginWindow *ui;

    // Helper function for hashing passwords
    QString hashPassword(const QString& password) const;
};

#endif // LOGINWINDOW_H
