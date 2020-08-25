#include "gamelogic.h"
#include <functional>
GameLogic::GameLogic(QObject *parent) : QObject(parent)
{
    connect(&m_timer,&QTimer::timeout,this,&GameLogic::update);
}

void GameLogic::startGame()
{
    resetMoves();
}

void GameLogic::endTurn(bool team)
{
    m_dataSource->setTeamTurn(!team);
    if(team) m_dataSource->setTurn(m_dataSource->turn()+1);
    resetMoves();
    waitForTurn(team);
}
void GameLogic::waitForTurn(bool team)
{
    if(m_dataSource->teamTurn()!=team){
        team ? m_dataSource->setInfo("Rozpoczynanie tury niebieskich") : m_dataSource->setInfo("Rozpoczynanie tury czerwonych");
        m_teamTurn=!team;
        m_timer.start(3000);
    }
}

void GameLogic::resetTroopsPosition(bool team)
{
    quint8 newRow;
    team? newRow=3:newRow=12;
    QJsonValue jNewRow(newRow);
    for(int i=0;i<m_dataSource->dataItems(team).size();i++){
        QJsonValue jNewColumn(i+3);
        m_dataSource->setTokenIn(m_dataSource->getStat<quint8>(i,DataSource::row,team),m_dataSource->getStat<quint8>(i,DataSource::col,team),false);
        m_dataSource->setStat(i,DataSource::row,team,jNewRow);
        m_dataSource->setStat(i,DataSource::col,team,jNewColumn);
    }
    emit dataChanged();
}

void GameLogic::attack(quint8 attackerIndex,const QString& defenderName,bool team)
{
    Troop *attackerTroop = m_dataSource->dataItems(team).at(attackerIndex);
    QString message="";
    quint8 attacks = attackerTroop->statList().at(DataSource::attacks).toInt();             //pobranie liczby ataków atakującego
    if(attacks==0) {
        m_dataSource->setInfo("brak możliwości ataku");
    }else{

        quint8 attackValue = attackerTroop->statList().at(DataSource::atkVal).toInt();       //pobieranie wartości ataku
        quint8 dmg = attackerTroop->statList().at(DataSource::dmgVal).toInt();               //pobieranie obrażeń
        quint8 dmgDice = attackerTroop->statList().at(DataSource::dmgDice).toInt();           //pobieranie kości obrażeń
        QString attackerName=attackerTroop->statList().at(DataSource::name).toString();   //pobieranie nazwy atakującego
        QJsonValue jAttacks(attacks-1);                                                                      //zmniejszenie liczby ataków atakującego po ataku
        if(attacks==1) {                                                                            //jeśli ataki się wyczerpały jednosta nie może się poruszać
            QJsonValue jMoved(true);
            attackerTroop->setStatList(DataSource::moved,jMoved);
        }
        attackerTroop->setStatList(DataSource::attacks,jAttacks);                           //zmiana liczby ataków atakującego w datasource
        //atak:
        for(auto &troop : m_dataSource->dataItems(!team)){
            if(troop->statList().at(DataSource::name).toString()==defenderName){
                quint8 defence = troop->statList().at(DataSource::def).toInt();
                quint8 attackRoll=QRandomGenerator::global()->bounded(1,20);
                qint8 attackTest = attackRoll +attackValue - defence;
                if(attackTest>=0) {
                    message=message+attackerName+ " trafia "+ defenderName+"!: " + QString::number(attackRoll) +"+" +QString::number(attackValue)+"="+QString::number(attackRoll+attackValue)+ " vs: " +QString::number(defence);
                    quint8 dmgDealt = QRandomGenerator::global()->bounded(1,dmgDice)+dmg;
                    message=message+"\r\n zadano "+QString::number(dmgDealt)+" obrażeń ";
                    quint8 hp=troop->statList().at(DataSource::hp).toInt();
                    if(hp<=dmgDealt){
                        message=message+"\r\n"+defenderName+" zniszczona!";
                        QJsonValue troopActive(false);
                        troop->setStatList(DataSource::active,troopActive);                  //zmiana statusu jednostki na nieaktywny
                    }
                    QJsonValue jsonDmg(hp-dmgDealt);
                    troop->setStatList(DataSource::hp,jsonDmg);                          //zmiana hp jednostyki
                }else {
                    message=message+attackerName+ " pudłuje "+defenderName+": " + QString::number(attackRoll) +"+" +QString::number(attackValue)+"="+QString::number(attackRoll+attackValue)+ " vs: " +QString::number(defence);
                }
            }
        }
        m_dataSource->writeConsole(message);
        emit dataChanged();
    }
}

void GameLogic::setDataSource(DataSource *ds)
{
    m_dataSource=ds;
}

void GameLogic::resetMoves()
{
    QJsonValue newMoves(false);
    QJsonValue newAttacks(2);
    for(auto &troop : m_dataSource->dataItems(true)){
        troop->setStatList(DataSource::moved,newMoves);
        troop->setStatList(DataSource::attacks,newAttacks);
    }
    for(auto &troop : m_dataSource->dataItems(false)){
        troop->setStatList(DataSource::moved,newMoves);
        troop->setStatList(DataSource::attacks,newAttacks);
    }

    emit dataChanged();
}
void GameLogic::resetTroop(quint8 index, bool team)
{
    QJsonValue hp= m_dataSource->getStat<quint8>(index,DataSource::maxHp,team);
    m_dataSource->setStat(index,DataSource::hp,team,hp);
    QJsonValue troopActive(true);
     m_dataSource->setStat(index,DataSource::active,team,troopActive);
    emit dataChanged();
}
QStringList GameLogic::findEnemy(quint8 row, quint8 col, quint8 range, bool team) const
{
    QStringList enemy;
        
    for(auto &troop : m_dataSource->dataItems(team)){
        int enemyRow = troop->statList().at(DataSource::row).toInt();
        int enemyCol = troop->statList().at(DataSource::col).toInt();
        if(qAbs(enemyRow-row)<=range && qAbs(enemyCol-col)<=range &&
                (qAbs(enemyRow-row)+qAbs(enemyCol-col)<range*2)){
            enemy.append(troop->statList().at(DataSource::name).toString());
        }
    }
    return enemy;
}

void GameLogic::removeTroop(bool team, quint8 index)
{
    m_dataSource->removeTroop(team,index);

    emit dataChanged();
}

void GameLogic::addTroop(bool aTeam,quint8 index)
{
    Troop *troop = new Troop(this);
    quint8 sRow;
    QJsonValue name("");
    QJsonValue type("");
    QJsonValue maxHp(20);
    QJsonValue aVal(2);
    QJsonValue def(10);
    QJsonValue dDice(6);
    QJsonValue dmg(0);
    QJsonValue speed(3);
    QJsonValue range(1);
    QJsonValue aCharge(0);
    QJsonValue aCrowd(0);
    QJsonValue morale(0);
    QJsonValue aType("");
    QJsonValue str(0);
    QJsonValue agi(0);
    QJsonValue end(0);
    QJsonValue will(0);
    aTeam?  sRow=0: sRow=14;
    QJsonValue row(sRow);
    QJsonValue col(3+index);
    QJsonValue id(index);
    QJsonValue team(aTeam);
    QJsonValue moved(false);
    QJsonValue attacks(2) ;
    QJsonValue active(true);
    QJsonValue hp(0);
    QJsonValue blank3(0);

    troop->setStatList(DataSource::name,name);
    troop->setStatList(DataSource::type,type);
    troop->setStatList(DataSource::maxHp,maxHp);
    troop->setStatList(DataSource::atkVal,aVal);
    troop->setStatList(DataSource::def,def);
    troop->setStatList(DataSource::dmgDice,dDice);
    troop->setStatList(DataSource::dmgVal,dmg);
    troop->setStatList(DataSource::speed,speed);
    troop->setStatList(DataSource::range,range);
    troop->setStatList(DataSource::atkCharg,aCharge);
    troop->setStatList(DataSource::atkCrowd,aCrowd);
    troop->setStatList(DataSource::morale,morale);
    troop->setStatList(DataSource::atkType,aType);
    troop->setStatList(DataSource::str,str);
    troop->setStatList(DataSource::agi,agi);
    troop->setStatList(DataSource::end,end);
    troop->setStatList(DataSource::will,will);
    troop->setStatList(DataSource::row,row);
    troop->setStatList(DataSource::col,col);
    troop->setStatList(DataSource::id,id);
    troop->setStatList(DataSource::team,team);
    troop->setStatList(DataSource::moved,moved);
    troop->setStatList(DataSource::attacks,attacks);
    troop->setStatList(DataSource::active,active);
    troop->setStatList(DataSource::hp,hp);
    troop->setStatList(DataSource::blank,blank3);
    m_dataSource->setTokenIn(row.toInt(),col.toInt(),true);
    m_dataSource->addTroop(troop,aTeam);
    emit dataChanged();
}

void GameLogic::startNewGame()
{
    m_dataSource->setTurn(1);
    m_dataSource->setTeamTurn(true);
    resetMoves();
    emit dataChanged();
}

void GameLogic::resetAllTroops(bool team)
{
    QJsonValue newMoves(false);
    QJsonValue newAttacks(2);
    QJsonValue troopActive(true);
       for(auto &troop : m_dataSource->dataItems(team)){
        QJsonValue hp= troop->statList().at(2);
        troop->setStatList(DataSource::hp,hp);
        troop->setStatList(DataSource::active,troopActive);
    }
    resetMoves();
     emit dataChanged();
}

void GameLogic::removeAllTroops(bool team)
{
    m_dataSource->dataItems(team).clear();
    emit dataChanged();
}
void GameLogic::update()
{
    if(m_dataSource->teamTurn()!=m_teamTurn) {
        m_timer.stop();
        m_teamTurn ? m_dataSource->setInfo("Rozpoczynanie tury niebieskich") : m_dataSource->setInfo("Rozpoczynanie tury czerwonych");
    }
    m_dataSource->fetchTroops();
}

