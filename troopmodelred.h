#ifndef TABLEMODEL2_H
#define TABLEMODEL2_H

#include <QAbstractTableModel>
#include <QDebug>
#include "datasource.h"
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include "troop.h"

class TableModel2 : public QAbstractTableModel
{
    Q_OBJECT

public:
    explicit TableModel2(QObject *parent = nullptr);
    Q_PROPERTY(DataSource * dataSource READ dataSource WRITE setDataSource NOTIFY dataSourceChanged)
    enum TroopRoles{
        trHeaderRole=Qt::UserRole+1,
        trBodyRole=Qt::UserRole+2,
        trNameRole=Qt::UserRole+3,
        trSpeedRole=Qt::UserRole+4,
        trRowRole=Qt::UserRole+5,
        trColRole=Qt::UserRole+6,
        trMovedRole=Qt::UserRole+7,
        trUpdateRole=Qt::UserRole+8,
        trHpRole=Qt::UserRole+9,
        trActiveRole=Qt::UserRole+10
    };

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    bool setData(const QModelIndex &index, const QVariant &value,
                 int role = Qt::EditRole) override;
    Qt::ItemFlags flags(const QModelIndex& index) const override;
    QHash<int, QByteArray> roleNames() const override;

    Q_INVOKABLE QStringList findEnemy(int row, int col, int range, bool team) const;
    DataSource *dataSource() const;
    void setDataSource(DataSource * dataSource);
signals:
    void dataSourceChanged(DataSource * dataSource);
    void troopChaged();
public slots:
   Q_INVOKABLE void updateAll();
     Q_INVOKABLE void updateModel();
    Q_INVOKABLE void updateServer();

private:
    DataSource * m_dataSource;
    bool m_signalConnected=false;
    QHash<int,QString> m_header;
    bool m_isHeader;
    bool m_team=true;

};

#endif // TABLEMODEL2_H
