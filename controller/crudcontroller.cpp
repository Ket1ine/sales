#include "crudcontroller.h"
#include "../model/database.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QVariant>
#include <QDebug>

// ==================== Products ====================

QList<Product> CrudController::getAllProducts()
{
    QList<Product> list;
    QSqlQuery q(Database::instance().db());
    q.prepare("SELECT id_товара, Наименование, Оптовая_цена, Розничная_цена, Справочная_информация "
              "FROM Товары ORDER BY id_товара");

    if (!q.exec()) {
        qDebug() << "getAllProducts error:" << q.lastError().text();
        return list;
    }

    while (q.next()) {
        Product p;
        p.id = q.value(0).toInt();
        p.name = q.value(1).toString();
        p.wholesalePrice = q.value(2).toDouble();
        p.retailPrice = q.value(3).toDouble();
        p.info = q.value(4).toString();
        list.append(p);
    }

    return list;
}

bool CrudController::addProduct(const Product& p)
{
    QSqlQuery q(Database::instance().db());
    q.prepare("INSERT INTO Товары (Наименование, Оптовая_цена, Розничная_цена, Справочная_информация) "
              "VALUES (:name, :wp, :rp, :info)");
    q.bindValue(":name", p.name);
    q.bindValue(":wp", p.wholesalePrice);
    q.bindValue(":rp", p.retailPrice);
    q.bindValue(":info", p.info);

    if (!q.exec()) {
        qDebug() << "addProduct error:" << q.lastError().text();
        return false;
    }

    return true;
}

bool CrudController::updateProduct(const Product& p)
{
    QSqlQuery q(Database::instance().db());
    q.prepare("UPDATE Товары SET Наименование=:name, Оптовая_цена=:wp, Розничная_цена=:rp, "
              "Справочная_информация=:info WHERE id_товара=:id");
    q.bindValue(":name", p.name);
    q.bindValue(":wp", p.wholesalePrice);
    q.bindValue(":rp", p.retailPrice);
    q.bindValue(":info", p.info);
    q.bindValue(":id", p.id);

    if (!q.exec()) {
        qDebug() << "updateProduct error:" << q.lastError().text();
        return false;
    }

    return true;
}

bool CrudController::deleteProduct(int id)
{
    QSqlQuery q(Database::instance().db());
    q.prepare("DELETE FROM Товары WHERE id_товара=:id");
    q.bindValue(":id", id);

    if (!q.exec()) {
        qDebug() << "deleteProduct error:" << q.lastError().text();
        return false;
    }

    return true;
}

// ==================== Buyers ====================

QList<Buyer> CrudController::getAllBuyers()
{
    QList<Buyer> list;
    QSqlQuery q(Database::instance().db());
    q.prepare("SELECT id_покупателя, Наименование, Адрес, Номер_телефона, Контактное_лицо, id_пользователя "
              "FROM Покупатели ORDER BY id_покупателя");

    if (!q.exec()) {
        qDebug() << "getAllBuyers error:" << q.lastError().text();
        return list;
    }

    while (q.next()) {
        Buyer b;
        b.id = q.value(0).toInt();
        b.name = q.value(1).toString();
        b.address = q.value(2).toString();
        b.phone = q.value(3).toString();
        b.contactPerson = q.value(4).toString();
        b.userId = q.value(5).toInt();
        list.append(b);
    }

    return list;
}

bool CrudController::addBuyer(const Buyer& b)
{
    QSqlQuery q(Database::instance().db());
    q.prepare("INSERT INTO Покупатели (Наименование, Адрес, Номер_телефона, Контактное_лицо) "
              "VALUES (:name, :addr, :phone, :contact)");
    q.bindValue(":name", b.name);
    q.bindValue(":addr", b.address);
    q.bindValue(":phone", b.phone);
    q.bindValue(":contact", b.contactPerson);

    if (!q.exec()) {
        qDebug() << "addBuyer error:" << q.lastError().text();
        return false;
    }

    return true;
}

bool CrudController::updateBuyer(const Buyer& b)
{
    QSqlQuery q(Database::instance().db());
    q.prepare("UPDATE Покупатели SET Наименование=:name, Адрес=:addr, Номер_телефона=:phone, "
              "Контактное_лицо=:contact WHERE id_покупателя=:id");
    q.bindValue(":name", b.name);
    q.bindValue(":addr", b.address);
    q.bindValue(":phone", b.phone);
    q.bindValue(":contact", b.contactPerson);
    q.bindValue(":id", b.id);

    if (!q.exec()) {
        qDebug() << "updateBuyer error:" << q.lastError().text();
        return false;
    }

    return true;
}

bool CrudController::deleteBuyer(int id)
{
    QSqlQuery q(Database::instance().db());
    q.prepare("DELETE FROM Покупатели WHERE id_покупателя=:id");
    q.bindValue(":id", id);

    if (!q.exec()) {
        qDebug() << "deleteBuyer error:" << q.lastError().text();
        return false;
    }

    return true;
}

// ==================== Documents ====================

QList<Document> CrudController::getAllDocuments()
{
    QList<Document> list;
    QSqlQuery q(Database::instance().db());
    q.prepare("SELECT d.id_документа, d.Дата_покупки, d.id_покупателя, d.id_скидки, "
              "p.Наименование AS buyer_name, s.Описание AS discount_desc, "
              "COALESCE(s.Размер_скидки, 0) AS discount_size "
              "FROM Документы d "
              "JOIN Покупатели p ON d.id_покупателя = p.id_покупателя "
              "LEFT JOIN Скидки s ON d.id_скидки = s.id_скидки "
              "ORDER BY d.id_документа");

    if (!q.exec()) {
        qDebug() << "getAllDocuments error:" << q.lastError().text();
        return list;
    }

    while (q.next()) {
        Document doc;
        doc.id = q.value(0).toInt();
        doc.date = q.value(1).toDate();
        doc.buyerId = q.value(2).toInt();
        doc.discountId = q.value(3).isNull() ? 0 : q.value(3).toInt();
        doc.buyerName = q.value(4).toString();
        doc.discountDesc = q.value(5).toString();
        doc.discountSize = q.value(6).toDouble();
        list.append(doc);
    }

    return list;
}

bool CrudController::addDocument(Document& d)
{
    QSqlQuery q(Database::instance().db());
    q.prepare("INSERT INTO Документы (Дата_покупки, id_покупателя, id_скидки) "
              "VALUES (:date, :buyer, :discount)");
    q.bindValue(":date", d.date);
    q.bindValue(":buyer", d.buyerId);
    q.bindValue(":discount", d.discountId == 0 ? QVariant() : d.discountId);

    if (!q.exec()) {
        qDebug() << "addDocument error:" << q.lastError().text();
        return false;
    }

    d.id = q.lastInsertId().toInt();
    return true;
}

bool CrudController::updateDocument(const Document& d)
{
    QSqlQuery q(Database::instance().db());
    q.prepare("UPDATE Документы SET Дата_покупки=:date, id_покупателя=:buyer, id_скидки=:discount "
              "WHERE id_документа=:id");
    q.bindValue(":date", d.date);
    q.bindValue(":buyer", d.buyerId);
    q.bindValue(":discount", d.discountId == 0 ? QVariant() : d.discountId);
    q.bindValue(":id", d.id);

    if (!q.exec()) {
        qDebug() << "updateDocument error:" << q.lastError().text();
        return false;
    }

    return true;
}

bool CrudController::deleteDocument(int id)
{
    QSqlQuery q(Database::instance().db());
    q.prepare("DELETE FROM Документы WHERE id_документа=:id");
    q.bindValue(":id", id);

    if (!q.exec()) {
        qDebug() << "deleteDocument error:" << q.lastError().text();
        return false;
    }

    return true;
}

// ==================== Discounts ====================

QList<Discount> CrudController::getAllDiscounts()
{
    QList<Discount> list;
    QSqlQuery q(Database::instance().db());
    q.prepare("SELECT id_скидки, Размер_скидки, Описание FROM Скидки ORDER BY id_скидки");

    if (!q.exec()) {
        qDebug() << "getAllDiscounts error:" << q.lastError().text();
        return list;
    }

    while (q.next()) {
        Discount d;
        d.id = q.value(0).toInt();
        d.size = q.value(1).toDouble();
        d.description = q.value(2).toString();
        list.append(d);
    }

    return list;
}

bool CrudController::addDiscount(const Discount& d)
{
    QSqlQuery q(Database::instance().db());
    q.prepare("INSERT INTO Скидки (Размер_скидки, Описание) VALUES (:size, :desc)");
    q.bindValue(":size", d.size);
    q.bindValue(":desc", d.description);

    if (!q.exec()) {
        qDebug() << "addDiscount error:" << q.lastError().text();
        return false;
    }

    return true;
}

bool CrudController::updateDiscount(const Discount& d)
{
    QSqlQuery q(Database::instance().db());
    q.prepare("UPDATE Скидки SET Размер_скидки=:size, Описание=:desc WHERE id_скидки=:id");
    q.bindValue(":size", d.size);
    q.bindValue(":desc", d.description);
    q.bindValue(":id", d.id);

    if (!q.exec()) {
        qDebug() << "updateDiscount error:" << q.lastError().text();
        return false;
    }

    return true;
}

bool CrudController::deleteDiscount(int id)
{
    QSqlQuery q(Database::instance().db());
    q.prepare("DELETE FROM Скидки WHERE id_скидки=:id");
    q.bindValue(":id", id);

    if (!q.exec()) {
        qDebug() << "deleteDiscount error:" << q.lastError().text();
        return false;
    }

    return true;
}

// ==================== Retail Sales ====================

QList<SaleItem> CrudController::getRetailSales(int docId)
{
    QList<SaleItem> list;
    QSqlQuery q(Database::instance().db());
    q.prepare("SELECT rp.id_документа, rp.id_товара, rp.Количество, t.Наименование, t.Розничная_цена "
              "FROM Розничные_продажи rp "
              "JOIN Товары t ON rp.id_товара = t.id_товара "
              "WHERE rp.id_документа = :docId");
    q.bindValue(":docId", docId);

    if (!q.exec()) {
        qDebug() << "getRetailSales error:" << q.lastError().text();
        return list;
    }

    while (q.next()) {
        SaleItem s;
        s.documentId = q.value(0).toInt();
        s.productId = q.value(1).toInt();
        s.quantity = q.value(2).toInt();
        s.productName = q.value(3).toString();
        s.price = q.value(4).toDouble();
        list.append(s);
    }

    return list;
}

bool CrudController::addRetailSale(const SaleItem& s)
{
    QSqlQuery q(Database::instance().db());
    q.prepare("INSERT INTO Розничные_продажи (id_документа, id_товара, Количество) "
              "VALUES (:doc, :prod, :qty)");
    q.bindValue(":doc", s.documentId);
    q.bindValue(":prod", s.productId);
    q.bindValue(":qty", s.quantity);

    if (!q.exec()) {
        qDebug() << "addRetailSale error:" << q.lastError().text();
        return false;
    }

    return true;
}

bool CrudController::deleteRetailSale(int docId, int productId)
{
    QSqlQuery q(Database::instance().db());
    q.prepare("DELETE FROM Розничные_продажи WHERE id_документа=:doc AND id_товара=:prod");
    q.bindValue(":doc", docId);
    q.bindValue(":prod", productId);

    if (!q.exec()) {
        qDebug() << "deleteRetailSale error:" << q.lastError().text();
        return false;
    }

    return true;
}

void CrudController::clearRetailSales(int docId)
{
    QSqlQuery q(Database::instance().db());
    q.prepare("DELETE FROM Розничные_продажи WHERE id_документа=:doc");
    q.bindValue(":doc", docId);

    if (!q.exec()) {
        qDebug() << "clearRetailSales error:" << q.lastError().text();
    }
}

// ==================== Wholesale Sales ====================

QList<SaleItem> CrudController::getWholesaleSales(int docId)
{
    QList<SaleItem> list;
    QSqlQuery q(Database::instance().db());
    q.prepare("SELECT op.id_документа, op.id_товара, op.Количество, t.Наименование, t.Оптовая_цена "
              "FROM Оптовые_продажи op "
              "JOIN Товары t ON op.id_товара = t.id_товара "
              "WHERE op.id_документа = :docId");
    q.bindValue(":docId", docId);

    if (!q.exec()) {
        qDebug() << "getWholesaleSales error:" << q.lastError().text();
        return list;
    }

    while (q.next()) {
        SaleItem s;
        s.documentId = q.value(0).toInt();
        s.productId = q.value(1).toInt();
        s.quantity = q.value(2).toInt();
        s.productName = q.value(3).toString();
        s.price = q.value(4).toDouble();
        list.append(s);
    }

    return list;
}

bool CrudController::addWholesaleSale(const SaleItem& s)
{
    QSqlQuery q(Database::instance().db());
    q.prepare("INSERT INTO Оптовые_продажи (id_документа, id_товара, Количество) "
              "VALUES (:doc, :prod, :qty)");
    q.bindValue(":doc", s.documentId);
    q.bindValue(":prod", s.productId);
    q.bindValue(":qty", s.quantity);

    if (!q.exec()) {
        qDebug() << "addWholesaleSale error:" << q.lastError().text();
        return false;
    }

    return true;
}

bool CrudController::deleteWholesaleSale(int docId, int productId)
{
    QSqlQuery q(Database::instance().db());
    q.prepare("DELETE FROM Оптовые_продажи WHERE id_документа=:doc AND id_товара=:prod");
    q.bindValue(":doc", docId);
    q.bindValue(":prod", productId);

    if (!q.exec()) {
        qDebug() << "deleteWholesaleSale error:" << q.lastError().text();
        return false;
    }

    return true;
}

void CrudController::clearWholesaleSales(int docId)
{
    QSqlQuery q(Database::instance().db());
    q.prepare("DELETE FROM Оптовые_продажи WHERE id_документа=:doc");
    q.bindValue(":doc", docId);

    if (!q.exec()) {
        qDebug() << "clearWholesaleSales error:" << q.lastError().text();
    }
}

// ==================== Users ====================

QList<User> CrudController::getAllUsers()
{
    QList<User> list;
    QSqlQuery q(Database::instance().db());
    q.prepare("SELECT u.id_пользователя, u.Логин, u.id_роли, r.Наименование_роли "
              "FROM Пользователи u JOIN Роли r ON u.id_роли = r.id_роли "
              "ORDER BY u.id_пользователя");

    if (!q.exec()) {
        qDebug() << "getAllUsers error:" << q.lastError().text();
        return list;
    }

    while (q.next()) {
        User u;
        u.id = q.value(0).toInt();
        u.login = q.value(1).toString();
        u.roleId = q.value(2).toInt();
        u.roleName = q.value(3).toString();
        list.append(u);
    }

    return list;
}

bool CrudController::deleteUser(int id)
{
    QSqlQuery q(Database::instance().db());
    q.prepare("DELETE FROM Пользователи WHERE id_пользователя=:id");
    q.bindValue(":id", id);

    if (!q.exec()) {
        qDebug() << "deleteUser error:" << q.lastError().text();
        return false;
    }

    return true;
}

bool CrudController::updateUserRole(int userId, int roleId)
{
    QSqlQuery q(Database::instance().db());
    q.prepare("UPDATE Пользователи SET id_роли=:role WHERE id_пользователя=:id");
    q.bindValue(":role", roleId);
    q.bindValue(":id", userId);

    if (!q.exec()) {
        qDebug() << "updateUserRole error:" << q.lastError().text();
        return false;
    }

    return true;
}

// ==================== Roles ====================

QList<Role> CrudController::getAllRoles()
{
    QList<Role> list;
    QSqlQuery q(Database::instance().db());
    q.prepare("SELECT id_роли, Наименование_роли FROM Роли ORDER BY id_роли");

    if (!q.exec()) {
        qDebug() << "getAllRoles error:" << q.lastError().text();
        return list;
    }

    while (q.next()) {
        Role r;
        r.id = q.value(0).toInt();
        r.name = q.value(1).toString();
        list.append(r);
    }

    return list;
}
