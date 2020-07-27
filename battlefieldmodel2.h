#ifndef BATTLEFIELDMODEL2_H
#define BATTLEFIELDMODEL2_H

#include <QAbstractTableModel>
#include "datasource.h"
class BattlefieldModel2 : public QAbstractTableModel
{
    Q_OBJECT

public:
    enum cellRoles{
        cellAvalibleRole=Qt::UserRole+1,
        cellTokenInRole=Qt::UserRole+2,
        cellColorRole=Qt::UserRole+3

    };
    enum cellColors{
        cellColorTrans=0,
        cellColorGray=1,
        cellCorolRed=2
    };
    explicit BattlefieldModel2(QObject *parent = nullptr);

    // Basic functionality:
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    bool setData(const QModelIndex &index, const QVariant &value,
                 int role = Qt::EditRole) override;
    Qt::ItemFlags flags(const QModelIndex& index) const override;
    QHash<int, QByteArray> roleNames() const override;

    void setDataSource(DataSource * dataSource);
    Q_INVOKABLE void colorCells(int row,int col, int speed,int color,int moved);
     Q_INVOKABLE void cellAvalible(short row,short col, bool aval);
    Q_INVOKABLE void updateModel();
private:
    DataSource  *m_dataSource;

    int m_cellRows;
    int m_cellColumns;



};

#endif // BATTLEFIELDMODEL2_H
