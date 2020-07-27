#include "battlefieldmodel2.h"
#include <math.h>

BattlefieldModel2::BattlefieldModel2(QObject *parent)
    : QAbstractTableModel(parent)
{

}

int BattlefieldModel2::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;

    // FIXME: Implement me!
    return m_dataSource->getCellRowCount();
}

int BattlefieldModel2::columnCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;
    return m_dataSource->getCellColumnCount();
}

QVariant BattlefieldModel2::data(const QModelIndex &index, int role) const
{
    if (!index.isValid()){
        qInfo() << "index not valid. battlefield";
        return QVariant();
    }
    if(role==cellTokenInRole){
        return m_dataSource->getTokenIn(index.row(),index.column());
    }else if(role==cellColorRole){
        return m_dataSource->getCellColor(index.row(),index.column());
    }else{
        return QVariant();
    }
    return QVariant();
}

bool BattlefieldModel2::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (data(index, role) != value) {
        if(role==cellTokenInRole){
            m_dataSource->setTokenIn(index.row(),index.column(),value.toBool());
            emit dataChanged(index,index, {role, cellColorRole});
            return true;
        }else if(role==cellColorRole){
            m_dataSource->setCellColor(index.row(),index.column(),value.toString());
            emit dataChanged(index,index, QVector<int>() << role);
            return true;
        }else{
            qInfo() << "bad battlemodel token";
            return false;
        }
        return false;
    }
    return false;
}

Qt::ItemFlags BattlefieldModel2::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::NoItemFlags;
    return Qt::ItemIsEditable; // FIXME: Implement me!
}

QHash<int, QByteArray> BattlefieldModel2::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[cellAvalibleRole]="avalibleRole";
    roles[cellTokenInRole]="tokenInRole";
    roles[cellColorRole]="colorRole";
    return  roles;
}

void BattlefieldModel2::setDataSource(DataSource *dataSource)
{
    m_dataSource = dataSource;
}
void BattlefieldModel2::colorCells(int row, int col, int speed, int color,int moved)
{
    m_cellRows = m_dataSource->getCellRowCount();
    m_cellColumns =m_dataSource->getCellColumnCount();
    if(moved==1) speed=0;
    short iZ=-speed;
    short jZ=-speed;
    for(short i=row-speed;i<=row+speed;i++){
        for(short j=col-speed;j<=col+speed;j++){
            if(qAbs(jZ)+qAbs(iZ)<=speed+1 && j>=0 && i>=0&&i<m_cellRows&&j<m_cellColumns){
                QModelIndex index = createIndex(i,j);
                switch (color) {
                case cellColorTrans:{
                    setData(index,"transparent",cellColorRole);
                    break;
                }
                case cellColorGray:{
                    if(m_dataSource->getTokenIn(i,j)==true && (i!=row || j!=col)){
                        setData(index,"red",cellColorRole);
                    }else if(m_dataSource->getTokenIn(i,j)==true && i==row && j==col){
                        setData(index,"yellow",cellColorRole);
                    }else{
                        setData(index,"grey",cellColorRole);
                    }
                    break;
                }
                case cellCorolRed:{
                    setData(index,"red",cellColorRole);
                    break;
                }
                }
            }
            jZ++;
        }
        jZ=-speed;
        iZ++;
    }
}

void BattlefieldModel2::cellAvalible(short row, short col, bool aval)
{
   // QModelIndex index = createIndex(row,col);
    //setData(index,aval,cellTokenInRole);
    m_dataSource->setTokenIn(row,col,aval);

}

void BattlefieldModel2::updateModel()
{
    emit beginResetModel();
    emit endResetModel();           //resetuje model
}



