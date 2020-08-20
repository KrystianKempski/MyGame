#include "troopmodelred.h"

TableModel2::TableModel2(QObject *parent)
    : QAbstractTableModel(parent)
{
}

int TableModel2::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return m_dataSource->dataItems(m_team).size();
}

int TableModel2::columnCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;
    return 9;
}

QVariant TableModel2::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();
    switch (role) {
    case trHeaderRole:
        return m_header[index.column()];
    case trBodyRole:
        return m_dataSource->getStat<QJsonValue>(index.row(),index.column(),m_team).toVariant();
    case trNameRole:
        return m_dataSource->getStat<QJsonValue>(index.row(),DataSource::name,m_team).toVariant();
    case trSpeedRole:
        return m_dataSource->getStat<QJsonValue>(index.row(),DataSource::speed,m_team).toVariant();
    case trRowRole:
        return m_dataSource->getStat<QJsonValue>(index.row(),DataSource::row,m_team).toVariant();
    case trColRole:
        return m_dataSource->getStat<QJsonValue>(index.row(),DataSource::col,m_team).toVariant();
    case trMovedRole:
        return m_dataSource->getStat<QJsonValue>(index.row(),DataSource::moved,m_team).toVariant();
    case trHpRole:
        return m_dataSource->getStat<QJsonValue>(index.row(),DataSource::hp,m_team).toVariant();
    case trActiveRole:
        return m_dataSource->getStat<QJsonValue>(index.row(),DataSource::active,m_team).toVariant();
    case trRangeRole:
        return m_dataSource->getStat<QJsonValue>(index.row(),DataSource::range,m_team).toVariant();
    case trTypeRole:
        return m_dataSource->getStat<QJsonValue>(index.row(),DataSource::type,m_team).toVariant();
    case trAttacksRole:
        return m_dataSource->getStat<QJsonValue>(index.row(),DataSource::attacks,m_team).toVariant();
    case trAttackValRole:
        return m_dataSource->getStat<QJsonValue>(index.row(),DataSource::atkVal,m_team).toVariant();
    case trDefenceRole:
        return m_dataSource->getStat<QJsonValue>(index.row(),DataSource::def,m_team).toVariant();
    case trDmgDiceRole:
        return m_dataSource->getStat<QJsonValue>(index.row(),DataSource::dmgDice,m_team).toVariant();
    case trDmgValRole:
        return m_dataSource->getStat<QJsonValue>(index.row(),DataSource::dmgVal,m_team).toVariant();
    default:
        qInfo() << "błąd roli!";
        qInfo() << role;
        break;
    }
    qInfo() << "data failed to return";
    return QVariant();
}

bool TableModel2::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if(index.row()<0 || index.row()>=m_dataSource->dataItems(m_team).size()) return false;
    QJsonValue temp(value.toJsonValue());
    switch (role)
    {
    case trBodyRole:
    {
        m_dataSource->setStat(index.row(),index.column(),m_team,temp);
        emit dataChanged(index,index, {role, trNameRole,trHpRole,trActiveRole,trRowRole,trColRole});
        return true;

    }
    case trRowRole:
    {
        m_dataSource->setStat(index.row(),DataSource::row,m_team,temp);
        emit dataChanged(index,index, QVector<int>() << role);
        return true;
    }
    case trColRole:
    {
        m_dataSource->setStat(index.row(),DataSource::col,m_team,temp);
        emit dataChanged(index,index, QVector<int>() << role);
        return true;
    }
    case trMovedRole:
    {
        m_dataSource->setStat(index.row(),DataSource::moved,m_team,temp);
        emit dataChanged(index,index, QVector<int>() << role);
        return true;
    }
    case trAttacksRole:
    {
        m_dataSource->setStat(index.row(),DataSource::attacks,m_team,temp);
        emit dataChanged(index,index, QVector<int>() << role);
        return true;
    }
    default:
        return false;
    }
}

Qt::ItemFlags TableModel2::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::NoItemFlags;
    return Qt::ItemIsEditable;
}

QHash<int, QByteArray> TableModel2::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[trHeaderRole]="header";
    roles[trBodyRole]="troops";
    roles[trNameRole]="name";
    roles[trSpeedRole]="speed";
    roles[trRowRole]="rowTr";
    roles[trColRole]="colTr";
    roles[trMovedRole]="moved";
    roles[trHpRole]="hp";
    roles[trActiveRole]="active";
    roles[trRangeRole]="range";
    roles[trTypeRole]="type";
    roles[trAttacksRole]="attacks";
    roles[trAttackValRole]="attackVal";
    roles[trDefenceRole]="defence";
    roles[trDmgDiceRole]="dmgDice";
    roles[trDmgValRole]="dmgVal";

    return roles;
}

DataSource *TableModel2::dataSource() const
{
    return m_dataSource;
}

void TableModel2::setDataSource(DataSource *dataSource)
{
    beginResetModel();

    if(m_dataSource && m_signalConnected) m_dataSource->disconnect(this);
    m_dataSource = dataSource;


    connect(m_dataSource,&DataSource::preInsertTroopRed,this,[=](){
        const int index = m_dataSource->dataItems(m_team).size();
        beginInsertRows(QModelIndex(),index,index);
    });

    connect(m_dataSource,&DataSource::postInsertTroopRed,this,[=](){
        endInsertRows();
    });

    connect(m_dataSource,&DataSource::preRemoveTroopRed,this,[=](int index){
        beginRemoveRows(QModelIndex(),index,index);
    });

    connect(m_dataSource,&DataSource::postRemoveTroopRed,this,[=](){
        endRemoveRows();
    });
    m_signalConnected = true;
    endResetModel();
}

void TableModel2::updateAll()
{
    emit beginResetModel();
    emit endResetModel();           //resetuje model
    emit troopChaged();             //resetuje plik json na serwerze
}
void TableModel2::updateModel()
{
    emit beginResetModel();
    emit endResetModel();           //resetuje model
}

void TableModel2::updateServer()
{
    emit troopChaged();
}






