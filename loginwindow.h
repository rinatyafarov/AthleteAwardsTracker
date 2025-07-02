#ifndef LOGINWINDOW_H
#define LOGINWINDOW_H

#include <QDialog>
#include <QCryptographicHash> // For password hashing

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
    void loginSuccessful(const QString& login);

private slots:
    void on_loginButton_clicked();

private:
    Ui::LoginWindow *ui;

    // Helper function for hashing passwords
    QString hashPassword(const QString& password) const;
};

#endif // LOGINWINDOW_H
