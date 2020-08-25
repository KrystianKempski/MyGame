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
    for(int i=0;i<m_dataSource->dataItems(team).size();i++){
        quint8 newColumn = i+3;
        m_dataSource->setTokenIn(m_dataSource->getStat<quint8>(i,DataSource::row,team),m_dataSource->getStat<quint8>(i,DataSource::col,team),false);
        m_dataSource->setStat(i,DataSource::row,team,newRow);
        m_dataSource->setStat(i,DataSource::col,team,newColumn);
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
        if(--attacks==1) {                                                                           //zmniejszenie liczby ataków atakującego po ataku jeśli ataki się wyczerpały jednosta nie może się poruszać
            attackerTroop->setStatList(DataSource::moved,true);
        }
        attackerTroop->setStatList(DataSource::attacks,attacks);                           //zmiana liczby ataków atakującego w datasource
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
                        troop->setStatList(DataSource::active,false);                  //zmiana statusu jednostki na nieaktywny
                    }
                    troop->setStatList(DataSource::hp,hp-dmgDealt);                          //zmiana hp jednostyki
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
    bool newMoves(false);
    quint8 newAttacks(2);
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
    m_dataSource->setStat(index,DataSource::hp,team,m_dataSource->getStat<quint8>(index,DataSource::maxHp,team));
     m_dataSource->setStat(index,DataSource::active,team,true);
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
    troop->setStatList(DataSource::name,"");
    troop->setStatList(DataSource::type,"");
    troop->setStatList(DataSource::maxHp,20);
    troop->setStatList(DataSource::atkVal,1);
    troop->setStatList(DataSource::def,10);
    troop->setStatList(DataSource::dmgDice,6);
    troop->setStatList(DataSource::dmgVal,1);
    troop->setStatList(DataSource::speed,3);
    troop->setStatList(DataSource::range,1);
    troop->setStatList(DataSource::atkCharg,0);
    troop->setStatList(DataSource::atkCrowd,0);
    troop->setStatList(DataSource::morale,0);
    troop->setStatList(DataSource::atkType,"");
    troop->setStatList(DataSource::str,0);
    troop->setStatList(DataSource::agi,0);
    troop->setStatList(DataSource::end,0);
    troop->setStatList(DataSource::will,0);
    troop->setStatList(DataSource::row,aTeam ? 0 : 14);
    troop->setStatList(DataSource::col,3+index);
    troop->setStatList(DataSource::id,index);
    troop->setStatList(DataSource::team,aTeam);
    troop->setStatList(DataSource::moved,false);
    troop->setStatList(DataSource::attacks,2);
    troop->setStatList(DataSource::active,true);
    troop->setStatList(DataSource::hp,20);
    troop->setStatList(DataSource::blank,0);
    m_dataSource->setTokenIn(aTeam ? 0 : 14,3+index,true);
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
       for(auto &troop : m_dataSource->dataItems(team)){
        troop->setStatList(DataSource::hp,troop->statList().at(DataSource::maxHp));
        troop->setStatList(DataSource::active,true);
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

