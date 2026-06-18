#ifndef AUTHCONTROLLER_H
#define AUTHCONTROLLER_H

#include "../model/models.h"
#include <QString>

class AuthController {
public:
    User login(const QString& login, const QString& password);
    bool registerUser(const QString& login, const QString& password, int roleId = 2);
    bool changePassword(int userId, const QString& oldPass, const QString& newPass);
    bool changeLogin(int userId, const QString& newLogin);
    static bool validatePassword(const QString& password);
    static QString hashPassword(const QString& password);
};

#endif
