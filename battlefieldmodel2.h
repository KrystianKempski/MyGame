#ifndef BATTLEFIELDMODEL2_H
#define BATTLEFIELDMODEL2_H

#include <QAbstractTableModel>

class BattlefieldModel2 : public QAbstractTableModel
{
    Q_OBJECT

public:
    explicit BattlefieldModel2(QObject *parent = nullptr);

    // Basic functionality:
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    // Editable:
    bool setData(const QModelIndex &index, const QVariant &value,
                 int role = Qt::EditRole) override;

    Qt::ItemFlags flags(const QModelIndex& index) const override;

private:
};

#endif // BATTLEFIELDMODEL2_H
