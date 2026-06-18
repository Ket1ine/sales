#ifndef MODELS_H
#define MODELS_H

#include <QString>
#include <QDate>

struct Role {
    int id = 0;
    QString name;
};

struct User {
    int id = 0;
    QString login;
    QString passwordHash;
    int roleId = 0;
    QString roleName;
};

struct Buyer {
    int id = 0;
    QString name;
    QString address;
    QString phone;
    QString contactPerson;
    int userId = 0;
};

struct Product {
    int id = 0;
    QString name;
    double wholesalePrice = 0.0;
    double retailPrice = 0.0;
    QString info;
};

struct Discount {
    int id = 0;
    double size = 0.0;
    QString description;
};

struct Document {
    int id = 0;
    QDate date;
    int buyerId = 0;
    int discountId = 0;
    QString buyerName;
    QString discountDesc;
    double discountSize = 0.0;
};

struct SaleItem {
    int documentId = 0;
    int productId = 0;
    int quantity = 0;
    QString productName;
    double price = 0.0;
};

#endif // MODELS_H
