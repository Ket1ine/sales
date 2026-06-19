#include "editdialog.h"

#include <QFormLayout>
#include <QVBoxLayout>
#include <QLineEdit>
#include <QDoubleSpinBox>
#include <QDateEdit>
#include <QComboBox>
#include <QDialogButtonBox>
#include <QMetaType>

EditDialog::EditDialog(const QString& title, const QStringList& labels,
                       const QVariantList& values, QWidget* parent)
    : QDialog(parent)
{
    setWindowTitle(title);

    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    m_formLayout = new QFormLayout;

    for (int i = 0; i < labels.size(); ++i) {
        QVariant val = (i < values.size()) ? values[i] : QVariant();
        QWidget* widget = nullptr;

        if (val.typeId() == QMetaType::Double || val.typeId() == QMetaType::Int) {
            QDoubleSpinBox* spin = new QDoubleSpinBox(this);
            spin->setRange(0, 999999.99);
            spin->setDecimals(2);
            spin->setValue(val.toDouble());
            widget = spin;
        } else if (val.typeId() == QMetaType::QDate) {
            QDateEdit* dateEdit = new QDateEdit(this);
            dateEdit->setCalendarPopup(true);
            QDate d = val.toDate();
            dateEdit->setDate(d.isValid() ? d : QDate::currentDate());
            widget = dateEdit;
        } else {
            QLineEdit* lineEdit = new QLineEdit(val.toString(), this);
            widget = lineEdit;
        }

        m_widgets.append(widget);
        m_formLayout->addRow(labels[i], widget);
    }

    mainLayout->addLayout(m_formLayout);

    m_buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
    connect(m_buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(m_buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);
    mainLayout->addWidget(m_buttonBox);
}

void EditDialog::setComboBox(int fieldIndex, const QStringList& items,
                             const QList<int>& ids, int currentId)
{
    if (fieldIndex < 0 || fieldIndex >= m_widgets.size())
        return;

    QComboBox* combo = new QComboBox(this);
    for (int i = 0; i < items.size(); ++i) {
        combo->addItem(items[i]);
    }
    m_comboIds[fieldIndex] = ids;

    if (currentId >= 0) {
        int idx = ids.indexOf(currentId);
        if (idx >= 0)
            combo->setCurrentIndex(idx);
    }

    QWidget* oldWidget = m_widgets[fieldIndex];
    m_formLayout->replaceWidget(oldWidget, combo);
    oldWidget->deleteLater();
    m_widgets[fieldIndex] = combo;
}

int EditDialog::getComboId(int fieldIndex) const
{
    if (!m_comboIds.contains(fieldIndex))
        return -1;

    QComboBox* combo = qobject_cast<QComboBox*>(m_widgets[fieldIndex]);
    if (!combo)
        return -1;

    const QList<int>& ids = m_comboIds[fieldIndex];
    int idx = combo->currentIndex();
    if (idx >= 0 && idx < ids.size())
        return ids[idx];
    return -1;
}

QVariantList EditDialog::getValues() const
{
    QVariantList result;
    for (int i = 0; i < m_widgets.size(); ++i) {
        if (QLineEdit* le = qobject_cast<QLineEdit*>(m_widgets[i])) {
            result.append(le->text());
        } else if (QDoubleSpinBox* sp = qobject_cast<QDoubleSpinBox*>(m_widgets[i])) {
            result.append(sp->value());
        } else if (QDateEdit* de = qobject_cast<QDateEdit*>(m_widgets[i])) {
            result.append(de->date());
        } else if (QComboBox* cb = qobject_cast<QComboBox*>(m_widgets[i])) {
            result.append(cb->currentText());
        } else {
            result.append(QVariant());
        }
    }
    return result;
}
