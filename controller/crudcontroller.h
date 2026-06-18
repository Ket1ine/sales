#ifndef CRUDCONTROLLER_H
#define CRUDCONTROLLER_H

#include "../model/models.h"
#include <QList>

class CrudController {
public:
    // Products (Товары)
    QList<Product> getAllProducts();
    bool addProduct(const Product& p);
    bool updateProduct(const Product& p);
    bool deleteProduct(int id);

    // Buyers (Покупатели)
    QList<Buyer> getAllBuyers();
    bool addBuyer(const Buyer& b);
    bool updateBuyer(const Buyer& b);
    bool deleteBuyer(int id);

    // Documents (Документы)
    QList<Document> getAllDocuments();
    bool addDocument(Document& d);
    bool updateDocument(const Document& d);
    bool deleteDocument(int id);

    // Discounts (Скидки)
    QList<Discount> getAllDiscounts();
    bool addDiscount(const Discount& d);
    bool updateDiscount(const Discount& d);
    bool deleteDiscount(int id);

    // Retail Sales (Розничные_продажи)
    QList<SaleItem> getRetailSales(int docId);
    bool addRetailSale(const SaleItem& s);
    bool deleteRetailSale(int docId, int productId);
    void clearRetailSales(int docId);

    // Wholesale Sales (Оптовые_продажи)
    QList<SaleItem> getWholesaleSales(int docId);
    bool addWholesaleSale(const SaleItem& s);
    bool deleteWholesaleSale(int docId, int productId);
    void clearWholesaleSales(int docId);

    // Users (Пользователи + Роли)
    QList<User> getAllUsers();
    bool deleteUser(int id);
    bool updateUserRole(int userId, int roleId);

    // Roles (Роли)
    QList<Role> getAllRoles();
};

#endif
