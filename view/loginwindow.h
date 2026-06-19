#ifndef LOGINWINDOW_H
#define LOGINWINDOW_H

#include <QWidget>
#include "../controller/authcontroller.h"

class QLineEdit;

class LoginWindow : public QWidget
{
    Q_OBJECT

public:
    explicit LoginWindow(QWidget* parent = nullptr);

private slots:
    void onLogin();
    void onRegister();

private:
    QLineEdit* m_loginEdit;
    QLineEdit* m_passwordEdit;
    AuthController m_auth;
};

#endif // LOGINWINDOW_H
