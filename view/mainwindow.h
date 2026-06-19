#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "../model/models.h"
#include "../controller/crudcontroller.h"

class QTabWidget;
class QTableWidget;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(const User& currentUser, QWidget* loginWindow,
                        QWidget* parent = nullptr);

private slots:
    void onAddProduct();
    void onEditProduct();
    void onDeleteProduct();

    void onAddBuyer();
    void onEditBuyer();
    void onDeleteBuyer();

    void onAddDocument();
    void onViewDocument();
    void onDeleteDocument();

    void onAddDiscount();
    void onEditDiscount();
    void onDeleteDiscount();

    void onDeleteUser();
    void onChangeUserRole();

    void onProfile();
    void onLogout();

private:
    QWidget* createProductsTab();
    QWidget* createBuyersTab();
    QWidget* createDocumentsTab();
    QWidget* createDiscountsTab();
    QWidget* createUsersTab();

    void loadProducts();
    void loadBuyers();
    void loadDocuments();
    void loadDiscounts();
    void loadUsers();

    User m_currentUser;
    QWidget* m_loginWindow;
    CrudController m_crud;

    QTabWidget* m_tabs;
    QTableWidget* m_productsTable;
    QTableWidget* m_buyersTable;
    QTableWidget* m_documentsTable;
    QTableWidget* m_discountsTable;
    QTableWidget* m_usersTable;
};

#endif // MAINWINDOW_H
