#ifndef EDITDIALOG_H
#define EDITDIALOG_H

#include <QDialog>
#include <QStringList>
#include <QVariantList>
#include <QList>
#include <QMap>
#include <QWidget>

class QFormLayout;
class QDialogButtonBox;

class EditDialog : public QDialog
{
    Q_OBJECT

public:
    explicit EditDialog(const QString& title, const QStringList& labels,
                        const QVariantList& values, QWidget* parent = nullptr);

    void setComboBox(int fieldIndex, const QStringList& items,
                     const QList<int>& ids, int currentId = -1);
    int getComboId(int fieldIndex) const;

    QVariantList getValues() const;

private:
    QList<QWidget*> m_widgets;
    QMap<int, QList<int>> m_comboIds;
    QFormLayout* m_formLayout;
    QDialogButtonBox* m_buttonBox;
};

#endif // EDITDIALOG_H
