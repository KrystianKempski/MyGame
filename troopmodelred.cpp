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
    return 9;//m_dataSource->dataItems().at(0)->statList().size();

}

QVariant TableModel2::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    Troop *troop = m_dataSource->dataItems(m_team).at(index.row());
    switch (role) {
    case trHeaderRole:
        return m_header[index.column()];
    case trBodyRole:
        return troop->statList().at(index.column()).toVariant();
    case trNameRole:
        return troop->statList().at(0).toVariant();
    case trSpeedRole:
        return troop->statList().at(7).toVariant();
    case trRowRole:
        return troop->statList().at(17).toVariant();
    case trColRole:
        return troop->statList().at(18).toVariant();
    case trMovedRole:
        return troop->statList().at(21).toVariant();
    case trUpdateRole:
        return troop->statList().at(21).toVariant();
    case trHpRole:
        return troop->statList().at(24).toVariant();
    case trActiveRole:
        return troop->statList().at(23).toVariant();
    case trRangeRole:
        return troop->statList().at(8).toVariant();
    case trTypeRole:
        return troop->statList().at(1).toVariant();
    case trAttacksRole:
        return troop->statList().at(22).toVariant();
    case trAttackValRole:
        return troop->statList().at(3).toVariant();
    case trDefenceRole:
        return troop->statList().at(4).toVariant();
    case trDmgDiceRole:
        return troop->statList().at(5).toVariant();
    case trDmgValRole:
        return troop->statList().at(6).toVariant();
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
    bool ok;
    Troop *troop = m_dataSource->dataItems(m_team).at(index.row());
    QJsonValue json;
    json = value.toInt(&ok);
    switch (role)
    {
    case trBodyRole:
    {
        if(!ok){
            if(value.toString()=="true"||value.toString()=="false"){
                json = value.toBool();
            }else{
                json =value.toString();
            }
        }
        if(troop->statList().at(index.column())!=json){
            troop->setStatList(index.column(),json);
            emit dataChanged(index,index, {role, trNameRole,trHpRole,trActiveRole,trRowRole,trColRole});
            //emit troopChaged();
            return true;
        }
        return false;
    }
    case trRowRole:
    {
        if(ok){
            if(troop->statList().at(17)!=json){
                troop->setStatList(17,json);
                emit dataChanged(index,index,QVector<int>() << role);
                return true;
            }
        }
        return false;
    }
    case trColRole:
    {
        if(ok){
            if(troop->statList().at(18)!=json){
                troop->setStatList(18,json);
                emit dataChanged(index,index,QVector<int>() << role);
                return true;
            }
        }
        return false;
    }
    case trMovedRole:
    {
        json=value.toBool();
        if(troop->statList().at(21)!=json){
            troop->setStatList(21,json);
            emit dataChanged(index,index,QVector<int>() << role);
            return true;
        }
        return false;
    }
    case trAttacksRole:
    {
        if(ok){
            if(troop->statList().at(22)!=json){
                troop->setStatList(22,json);
                emit dataChanged(index,index,QVector<int>() << role);
                return true;
            }
        }
        return false;
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

    connect(m_dataSource,&DataSource::preRemoveTroop,this,[=](int index){
        beginRemoveRows(QModelIndex(),index,index);
    });

    connect(m_dataSource,&DataSource::postRemoveTroop,this,[=](){
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






