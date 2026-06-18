#include "authcontroller.h"
#include "../model/database.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QCryptographicHash>
#include <QRegularExpression>
#include <QDebug>

QString AuthController::hashPassword(const QString& password)
{
    return QCryptographicHash::hash(password.toUtf8(), QCryptographicHash::Sha256).toHex();
}

bool AuthController::validatePassword(const QString& password)
{
    QRegularExpression re("^(?=.*[0-9])(?=.*[A-Z])(?=.*[!@#$%^&*]).{8,}$");
    return re.match(password).hasMatch();
}

User AuthController::login(const QString& login, const QString& password)
{
    User user;
    user.id = 0;

    QSqlQuery q(Database::instance().db());
    q.prepare("SELECT u.id_пользователя, u.Логин, u.Хеш_пароля, u.id_роли, r.Наименование_роли "
              "FROM Пользователи u JOIN Роли r ON u.id_роли = r.id_роли "
              "WHERE u.Логин = :login");
    q.bindValue(":login", login);

    if (!q.exec()) {
        qDebug() << "Login query error:" << q.lastError().text();
        return user;
    }

    if (q.next()) {
        QString storedHash = q.value(2).toString();
        if (storedHash == hashPassword(password)) {
            user.id = q.value(0).toInt();
            user.login = q.value(1).toString();
            user.passwordHash = storedHash;
            user.roleId = q.value(3).toInt();
            user.roleName = q.value(4).toString();
        }
    }

    return user;
}

bool AuthController::registerUser(const QString& login, const QString& password, int roleId)
{
    if (!validatePassword(password)) {
        qDebug() << "Password validation failed";
        return false;
    }

    QSqlQuery q(Database::instance().db());
    q.prepare("INSERT INTO Пользователи (Логин, Хеш_пароля, id_роли) VALUES (:login, :hash, :role)");
    q.bindValue(":login", login);
    q.bindValue(":hash", hashPassword(password));
    q.bindValue(":role", roleId);

    if (!q.exec()) {
        qDebug() << "Register error:" << q.lastError().text();
        return false;
    }

    return true;
}

bool AuthController::changePassword(int userId, const QString& oldPass, const QString& newPass)
{
    if (!validatePassword(newPass)) {
        qDebug() << "New password validation failed";
        return false;
    }

    QSqlQuery q(Database::instance().db());
    q.prepare("SELECT Хеш_пароля FROM Пользователи WHERE id_пользователя = :id");
    q.bindValue(":id", userId);

    if (!q.exec() || !q.next()) {
        qDebug() << "Change password select error:" << q.lastError().text();
        return false;
    }

    if (q.value(0).toString() != hashPassword(oldPass)) {
        qDebug() << "Old password mismatch";
        return false;
    }

    q.prepare("UPDATE Пользователи SET Хеш_пароля = :hash WHERE id_пользователя = :id");
    q.bindValue(":hash", hashPassword(newPass));
    q.bindValue(":id", userId);

    if (!q.exec()) {
        qDebug() << "Change password update error:" << q.lastError().text();
        return false;
    }

    return true;
}

bool AuthController::changeLogin(int userId, const QString& newLogin)
{
    QSqlQuery q(Database::instance().db());
    q.prepare("UPDATE Пользователи SET Логин = :login WHERE id_пользователя = :id");
    q.bindValue(":login", newLogin);
    q.bindValue(":id", userId);

    if (!q.exec()) {
        qDebug() << "Change login error:" << q.lastError().text();
        return false;
    }

    return true;
}
