#include "documentdialog.h"
#include "../controller/crudcontroller.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QComboBox>
#include <QDateEdit>
#include <QTableWidget>
#include <QHeaderView>
#include <QPushButton>
#include <QDialogButtonBox>
#include <QLabel>
#include <QSpinBox>
#include <QMessageBox>
#include <QDialog>

DocumentDialog::DocumentDialog(Document* existingDoc, CrudController* crud,
                               QWidget* parent)
    : QDialog(parent), m_crud(crud), m_existingDoc(existingDoc)
{
    setWindowTitle(existingDoc ? "Просмотр/Редактирование документа" : "Добавить документ");
    resize(500, 450);

    QVBoxLayout* mainLayout = new QVBoxLayout(this);

    // Form section
    QFormLayout* formLayout = new QFormLayout;

    m_buyerCombo = new QComboBox(this);
    m_buyers = m_crud->getAllBuyers();
    for (const Buyer& b : m_buyers) {
        m_buyerCombo->addItem(b.name);
    }
    formLayout->addRow("Покупатель:", m_buyerCombo);

    m_dateEdit = new QDateEdit(QDate::currentDate(), this);
    m_dateEdit->setCalendarPopup(true);
    formLayout->addRow("Дата:", m_dateEdit);

    m_discountCombo = new QComboBox(this);
    m_discountCombo->addItem("Нет скидки");
    m_discounts = m_crud->getAllDiscounts();
    for (const Discount& d : m_discounts) {
        m_discountCombo->addItem(d.description + " (" + QString::number(d.size) + "%)");
    }
    formLayout->addRow("Скидка:", m_discountCombo);

    m_typeCombo = new QComboBox(this);
    m_typeCombo->addItem("Розничная");
    m_typeCombo->addItem("Оптовая");
    formLayout->addRow("Тип продажи:", m_typeCombo);

    mainLayout->addLayout(formLayout);

    // Items section
    QLabel* itemsLabel = new QLabel("Товары:", this);
    mainLayout->addWidget(itemsLabel);

    m_itemsTable = new QTableWidget(this);
    m_itemsTable->setColumnCount(3);
    m_itemsTable->setHorizontalHeaderLabels({"Товар", "Количество", "Цена"});
    m_itemsTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_itemsTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_itemsTable->horizontalHeader()->setStretchLastSection(true);
    mainLayout->addWidget(m_itemsTable);

    // Item buttons
    QHBoxLayout* itemBtnLayout = new QHBoxLayout;
    QPushButton* addItemBtn = new QPushButton("Добавить товар", this);
    QPushButton* removeItemBtn = new QPushButton("Удалить товар", this);
    itemBtnLayout->addWidget(addItemBtn);
    itemBtnLayout->addWidget(removeItemBtn);
    mainLayout->addLayout(itemBtnLayout);

    connect(addItemBtn, &QPushButton::clicked, this, &DocumentDialog::onAddItem);
    connect(removeItemBtn, &QPushButton::clicked, this, &DocumentDialog::onRemoveItem);

    // Dialog buttons
    m_buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
    connect(m_buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(m_buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);
    mainLayout->addWidget(m_buttonBox);

    // Load products list
    m_products = m_crud->getAllProducts();

    // Edit mode: populate fields
    if (m_existingDoc) {
        m_dateEdit->setDate(m_existingDoc->date);

        // Set buyer combo
        for (int i = 0; i < m_buyers.size(); ++i) {
            if (m_buyers[i].id == m_existingDoc->buyerId) {
                m_buyerCombo->setCurrentIndex(i);
                break;
            }
        }

        // Set discount combo
        if (m_existingDoc->discountId == 0) {
            m_discountCombo->setCurrentIndex(0);
        } else {
            for (int i = 0; i < m_discounts.size(); ++i) {
                if (m_discounts[i].id == m_existingDoc->discountId) {
                    m_discountCombo->setCurrentIndex(i + 1);
                    break;
                }
            }
        }

        // Load items
        QList<SaleItem> retailItems = m_crud->getRetailSales(m_existingDoc->id);
        if (!retailItems.isEmpty()) {
            m_items = retailItems;
            m_typeCombo->setCurrentIndex(0);
        } else {
            QList<SaleItem> wholesaleItems = m_crud->getWholesaleSales(m_existingDoc->id);
            if (!wholesaleItems.isEmpty()) {
                m_items = wholesaleItems;
                m_typeCombo->setCurrentIndex(1);
            }
        }

        refreshItemsTable();
    }
}

void DocumentDialog::onAddItem()
{
    if (m_products.isEmpty()) {
        QMessageBox::warning(this, "Ошибка", "Нет доступных товаров");
        return;
    }

    QDialog dlg(this);
    dlg.setWindowTitle("Добавить товар");

    QFormLayout* layout = new QFormLayout(&dlg);

    QComboBox* productCombo = new QComboBox(&dlg);
    for (const Product& p : m_products) {
        productCombo->addItem(p.name);
    }
    layout->addRow("Товар:", productCombo);

    QSpinBox* qtySpin = new QSpinBox(&dlg);
    qtySpin->setRange(1, 9999);
    qtySpin->setValue(1);
    layout->addRow("Количество:", qtySpin);

    QDialogButtonBox* btnBox = new QDialogButtonBox(
        QDialogButtonBox::Ok | QDialogButtonBox::Cancel, &dlg);
    connect(btnBox, &QDialogButtonBox::accepted, &dlg, &QDialog::accept);
    connect(btnBox, &QDialogButtonBox::rejected, &dlg, &QDialog::reject);
    layout->addRow(btnBox);

    if (dlg.exec() == QDialog::Accepted) {
        int productIdx = productCombo->currentIndex();
        if (productIdx < 0 || productIdx >= m_products.size())
            return;

        const Product& prod = m_products[productIdx];

        SaleItem item;
        item.productId = prod.id;
        item.productName = prod.name;
        item.quantity = qtySpin->value();

        // Price based on sale type
        if (m_typeCombo->currentIndex() == 0) {
            item.price = prod.retailPrice;
        } else {
            item.price = prod.wholesalePrice;
        }

        m_items.append(item);
        refreshItemsTable();
    }
}

void DocumentDialog::onRemoveItem()
{
    int row = m_itemsTable->currentRow();
    if (row < 0 || row >= m_items.size()) {
        QMessageBox::warning(this, "Ошибка", "Выберите товар для удаления");
        return;
    }

    m_items.removeAt(row);
    refreshItemsTable();
}

void DocumentDialog::refreshItemsTable()
{
    m_itemsTable->setRowCount(0);
    for (int i = 0; i < m_items.size(); ++i) {
        int row = m_itemsTable->rowCount();
        m_itemsTable->insertRow(row);
        m_itemsTable->setItem(row, 0, new QTableWidgetItem(m_items[i].productName));
        m_itemsTable->setItem(row, 1, new QTableWidgetItem(QString::number(m_items[i].quantity)));
        m_itemsTable->setItem(row, 2, new QTableWidgetItem(QString::number(m_items[i].price, 'f', 2)));
    }
}

Document DocumentDialog::getDocument() const
{
    Document doc;
    if (m_existingDoc)
        doc.id = m_existingDoc->id;

    doc.date = m_dateEdit->date();

    int buyerIdx = m_buyerCombo->currentIndex();
    if (buyerIdx >= 0 && buyerIdx < m_buyers.size()) {
        doc.buyerId = m_buyers[buyerIdx].id;
        doc.buyerName = m_buyers[buyerIdx].name;
    }

    int discountIdx = m_discountCombo->currentIndex();
    if (discountIdx == 0) {
        doc.discountId = 0;
    } else {
        int dIdx = discountIdx - 1;
        if (dIdx >= 0 && dIdx < m_discounts.size()) {
            doc.discountId = m_discounts[dIdx].id;
            doc.discountDesc = m_discounts[dIdx].description;
            doc.discountSize = m_discounts[dIdx].size;
        }
    }

    return doc;
}

QList<SaleItem> DocumentDialog::getItems() const
{
    return m_items;
}

bool DocumentDialog::isRetail() const
{
    return m_typeCombo->currentIndex() == 0;
}
