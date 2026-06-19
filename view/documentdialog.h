#ifndef DOCUMENTDIALOG_H
#define DOCUMENTDIALOG_H

#include <QDialog>
#include <QList>
#include "../model/models.h"

class QComboBox;
class QDateEdit;
class QTableWidget;
class QDialogButtonBox;
class CrudController;

class DocumentDialog : public QDialog
{
    Q_OBJECT

public:
    explicit DocumentDialog(Document* existingDoc, CrudController* crud,
                            QWidget* parent = nullptr);

    Document getDocument() const;
    QList<SaleItem> getItems() const;
    bool isRetail() const;

private slots:
    void onAddItem();
    void onRemoveItem();

private:
    void refreshItemsTable();

    CrudController* m_crud;

    QComboBox* m_buyerCombo;
    QDateEdit* m_dateEdit;
    QComboBox* m_discountCombo;
    QComboBox* m_typeCombo;
    QTableWidget* m_itemsTable;
    QDialogButtonBox* m_buttonBox;

    QList<Buyer> m_buyers;
    QList<Product> m_products;
    QList<Discount> m_discounts;
    QList<SaleItem> m_items;

    Document* m_existingDoc;
};

#endif // DOCUMENTDIALOG_H
