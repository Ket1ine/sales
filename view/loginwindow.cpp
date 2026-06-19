#include "loginwindow.h"
#include "mainwindow.h"

#include <QLineEdit>
#include <QPushButton>
#include <QFormLayout>
#include <QVBoxLayout>
#include <QMessageBox>

LoginWindow::LoginWindow(QWidget* parent)
    : QWidget(parent)
{
    setWindowTitle("Авторизация");
    setFixedSize(350, 200);

    QVBoxLayout* mainLayout = new QVBoxLayout(this);

    QFormLayout* formLayout = new QFormLayout;

    m_loginEdit = new QLineEdit(this);
    formLayout->addRow("Логин:", m_loginEdit);

    m_passwordEdit = new QLineEdit(this);
    m_passwordEdit->setEchoMode(QLineEdit::Password);
    formLayout->addRow("Пароль:", m_passwordEdit);

    mainLayout->addLayout(formLayout);

    QPushButton* loginBtn = new QPushButton("Войти", this);
    connect(loginBtn, &QPushButton::clicked, this, &LoginWindow::onLogin);
    mainLayout->addWidget(loginBtn);

    QPushButton* registerBtn = new QPushButton("Регистрация", this);
    connect(registerBtn, &QPushButton::clicked, this, &LoginWindow::onRegister);
    mainLayout->addWidget(registerBtn);
}

void LoginWindow::onLogin()
{
    User user = m_auth.login(m_loginEdit->text(), m_passwordEdit->text());
    if (user.id > 0) {
        MainWindow* mw = new MainWindow(user, this);
        mw->show();
        this->hide();
    } else {
        QMessageBox::warning(this, "Ошибка", "Неверный логин или пароль");
    }
}

void LoginWindow::onRegister()
{
    QString password = m_passwordEdit->text();

    if (!AuthController::validatePassword(password)) {
        QMessageBox::warning(this, "Ошибка",
            "Пароль должен содержать минимум 8 символов, цифру, "
            "заглавную букву и спецсимвол (!@#$%^&*)");
        return;
    }

    if (m_auth.registerUser(m_loginEdit->text(), password)) {
        QMessageBox::information(this, "Успех", "Регистрация прошла успешно");
    } else {
        QMessageBox::warning(this, "Ошибка", "Не удалось зарегистрироваться");
    }
}
