/****************************************************************************
** Klasa TroopModelBlue odpowiada za stworzenie modelu armii jednej ze stron
** Wykorzystująć QAbstractTableModel, przechowuje informacje odnoścnie posz-
** czególnych oddziałow, oraz umożliwia eksportowanie ich do QML. Model akty-
** wnie śledzi zmiany wprowadzone przez użytkownika i aktualizuje je na serw-
** erze.
****************************************************************************/


#ifndef TABLEMODEL3_H
#define TABLEMODEL3_H

#include <QAbstractTableModel>
#include <QDebug>
#include "datasource.h"
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include "troop.h"

class TableModelBlue : public QAbstractTableModel
{
    Q_OBJECT

public:
    explicit TableModelBlue(QObject *parent = nullptr);
    Q_PROPERTY(DataSource * dataSource READ dataSource WRITE setDataSource NOTIFY dataSourceChanged)
    enum TroopRoles{
        trHeaderRole=Qt::UserRole+1,
        trBodyRole,
        trNameRole,
        trSpeedRole,
        trRowRole,
        trColRole,
        trMovedRole,
        trHpRole,
        trActiveRole,
        trRangeRole,
        trTypeRole,
        trAttacksRole,
        trAttackValRole,
        trDmgDiceRole,
        trDmgValRole,
        trDefenceRole
    };

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    bool setData(const QModelIndex &index, const QVariant &value,
                 int role = Qt::EditRole) override;
    Qt::ItemFlags flags(const QModelIndex& index) const override;
    QHash<int, QByteArray> roleNames() const override;
    DataSource *dataSource() const;
    void setDataSource(DataSource * dataSource);
signals:
    void dataSourceChanged(DataSource * dataSource);
    void troopChaged();
public slots:
    Q_INVOKABLE void updateAll();
    Q_INVOKABLE void updateServer();
    Q_INVOKABLE void updateModel();

private:
    DataSource * m_dataSource;
    bool m_signalConnected=false;
    QHash<int,QString> m_header;
    bool m_isHeader;
    bool m_team=false;

};

#endif // TABLEMODEL2_H
