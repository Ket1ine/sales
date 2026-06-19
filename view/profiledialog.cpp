#include "profiledialog.h"

#include <QVBoxLayout>
#include <QFormLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QMessageBox>
#include <QGroupBox>

ProfileDialog::ProfileDialog(User& currentUser, QWidget* parent)
    : QDialog(parent), m_user(currentUser)
{
    setWindowTitle("Профиль");

    QVBoxLayout* mainLayout = new QVBoxLayout(this);

    // Section 1: Change login
    QGroupBox* loginGroup = new QGroupBox("Изменить логин", this);
    QFormLayout* loginLayout = new QFormLayout(loginGroup);

    QLabel* currentLoginLabel = new QLabel("Текущий логин: " + m_user.login, this);
    loginLayout->addRow(currentLoginLabel);

    m_newLoginEdit = new QLineEdit(this);
    loginLayout->addRow("Новый логин:", m_newLoginEdit);

    QPushButton* changeLoginBtn = new QPushButton("Изменить логин", this);
    loginLayout->addRow(changeLoginBtn);
    connect(changeLoginBtn, &QPushButton::clicked, this, &ProfileDialog::onChangeLogin);

    mainLayout->addWidget(loginGroup);

    // Section 2: Change password
    QGroupBox* passGroup = new QGroupBox("Изменить пароль", this);
    QFormLayout* passLayout = new QFormLayout(passGroup);

    m_oldPasswordEdit = new QLineEdit(this);
    m_oldPasswordEdit->setEchoMode(QLineEdit::Password);
    passLayout->addRow("Старый пароль:", m_oldPasswordEdit);

    m_newPasswordEdit = new QLineEdit(this);
    m_newPasswordEdit->setEchoMode(QLineEdit::Password);
    passLayout->addRow("Новый пароль:", m_newPasswordEdit);

    QPushButton* changePassBtn = new QPushButton("Изменить пароль", this);
    passLayout->addRow(changePassBtn);
    connect(changePassBtn, &QPushButton::clicked, this, &ProfileDialog::onChangePassword);

    mainLayout->addWidget(passGroup);

    // Close button
    QPushButton* closeBtn = new QPushButton("Закрыть", this);
    connect(closeBtn, &QPushButton::clicked, this, &QDialog::accept);
    mainLayout->addWidget(closeBtn);
}

void ProfileDialog::onChangeLogin()
{
    QString newLogin = m_newLoginEdit->text().trimmed();
    if (newLogin.isEmpty()) {
        QMessageBox::warning(this, "Ошибка", "Введите новый логин");
        return;
    }

    if (m_auth.changeLogin(m_user.id, newLogin)) {
        m_user.login = newLogin;
        QMessageBox::information(this, "Успех", "Логин успешно изменён");
    } else {
        QMessageBox::warning(this, "Ошибка", "Не удалось изменить логин");
    }
}

void ProfileDialog::onChangePassword()
{
    QString oldPass = m_oldPasswordEdit->text();
    QString newPass = m_newPasswordEdit->text();

    if (!AuthController::validatePassword(newPass)) {
        QMessageBox::warning(this, "Ошибка",
            "Пароль должен содержать минимум 8 символов, цифру, "
            "заглавную букву и спецсимвол (!@#$%^&*)");
        return;
    }

    if (m_auth.changePassword(m_user.id, oldPass, newPass)) {
        QMessageBox::information(this, "Успех", "Пароль успешно изменён");
        m_oldPasswordEdit->clear();
        m_newPasswordEdit->clear();
    } else {
        QMessageBox::warning(this, "Ошибка", "Не удалось изменить пароль");
    }
}
