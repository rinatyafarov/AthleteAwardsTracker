#ifndef REGISTERDIALOG_H
#define REGISTERDIALOG_H

#include <QDialog>

namespace Ui {
class RegisterDialog;
}

class RegisterDialog : public QDialog
{
    Q_OBJECT

public:
    RegisterDialog(QWidget *parent = nullptr);
    ~RegisterDialog() override;

    QString getLogin() const;
    QString getPassword() const;

signals:
    void registrationSuccessful();

private slots:
    void on_registerButton_clicked();

private:
    Ui::RegisterDialog *ui;
};

#endif
