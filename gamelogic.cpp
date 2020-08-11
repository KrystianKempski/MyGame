#include "gamelogic.h"

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
    short newRow;
    team? newRow=3:newRow=12;
    QJsonValue jNewRow(newRow);
    for(int i=0;i<m_dataSource->dataItems(team).size();i++){
        QJsonValue jNewColumn(i+3);
        m_dataSource->setTokenIn(m_dataSource->dataItems(team).at(i)->statList().at(17).toInt(),m_dataSource->dataItems(team).at(i)->statList().at(18).toInt(),false);
        m_dataSource->dataItems(team).at(i)->setStatList(18,jNewColumn);
        m_dataSource->dataItems(team).at(i)->setStatList(17,jNewRow);
        //m_dataSource->setTokenIn(row.toInt(),col.toInt(),true);
    }
    emit dataChanged();
}

void GameLogic::attack(int attackerIndex, QString defenderName,bool team)
{
    QString message="";
    short attacks = m_dataSource->dataItems(team).at(attackerIndex)->statList().at(22).toInt();             //pobranie liczby ataków atakującego
    if(attacks==0) {
        m_dataSource->setInfo("brak możliwości ataku");
    }else{
        short attackValue = m_dataSource->dataItems(team).at(attackerIndex)->statList().at(3).toInt();       //pobieranie wartości ataku
        short dmg = m_dataSource->dataItems(team).at(attackerIndex)->statList().at(6).toInt();               //pobieranie obrażeń
        short dmgDice = m_dataSource->dataItems(team).at(attackerIndex)->statList().at(5).toInt();           //pobieranie kości obrażeń
        QString attackerName=m_dataSource->dataItems(team).at(attackerIndex)->statList().at(0).toString();   //pobieranie nazwy atakującego
        QJsonValue jAttacks(attacks-1);                                                                      //zmniejszenie liczby ataków atakującego po ataku
        if(jAttacks.toInt()==0) {                                                                            //jeśli ataki się wyczerpały jednosta nie może się poruszać
            QJsonValue jMoved(true);
            m_dataSource->dataItems(team).at(attackerIndex)->setStatList(21,jMoved);
        }
        m_dataSource->dataItems(team).at(attackerIndex)->setStatList(22,jAttacks);                           //zmiana liczby ataków atakującego w datasource
        //atak:
        for(int i =0;i<m_dataSource->dataItems(!team).size();i++){                                           //szukanie przeciwnika
            if(m_dataSource->dataItems(!team).at(i)->statList().at(0).toString()==defenderName){
                short defence = m_dataSource->dataItems(!team).at(i)->statList().at(4).toInt();
                short attackRoll=QRandomGenerator::global()->bounded(1,20);
                short attackTest = attackRoll +attackValue - defence;
                if(attackTest>=0) {
                    message=message+attackerName+ " trafia "+ defenderName+"!: " + QString::number(attackRoll) +"+" +QString::number(attackValue)+"="+QString::number(attackRoll+attackValue)+ " vs: " +QString::number(defence);
                    short dmgDealt = QRandomGenerator::global()->bounded(1,dmgDice)+dmg;
                    message=message+"\r\n zadano "+QString::number(dmgDealt)+" obrażeń ";
                    short hp= m_dataSource->dataItems(!team).at(i)->statList().at(24).toInt();
                    if(hp<=dmgDealt){
                        message=message+"\r\n"+defenderName+" zniszczona!";
                        QJsonValue troopActive(false);
                        m_dataSource->dataItems(!team).at(i)->setStatList(23,troopActive);                  //zmiana statusu jednostki na nieaktywny
                    }
                    QJsonValue jsonDmg(hp-dmgDealt);
                    m_dataSource->dataItems(!team).at(i)->setStatList(24,jsonDmg);                          //zmiana hp jednostyki
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
    for(int i=0;i<m_dataSource->dataItems(true).size();i++){
        m_dataSource->dataItems(true).at(i)->setStatList(21,newMoves);
        m_dataSource->dataItems(true).at(i)->setStatList(22,newAttacks);
    }
    for(int i=0;i<m_dataSource->dataItems(false).size();i++){
        m_dataSource->dataItems(false).at(i)->setStatList(21,newMoves);
        m_dataSource->dataItems(false).at(i)->setStatList(22,newAttacks);
    }

    emit dataChanged();
}
void GameLogic::resetTroop(short index, bool team)
{
    QJsonValue hp= m_dataSource->dataItems(team).at(index)->statList().at(2);
    m_dataSource->dataItems(team).at(index)->setStatList(24,hp);
    QJsonValue troopActive(true);
    m_dataSource->dataItems(team).at(index)->setStatList(23,troopActive);
    emit dataChanged();
}
QStringList GameLogic::findEnemy(int row, int col, int range, bool team) const
{
    QStringList enemy;
    for(int i=0;i<m_dataSource->dataItems(team).size();i++){
        int enemyRow = m_dataSource->dataItems(team).at(i)->statList().at(17).toInt();
        int enemyCol = m_dataSource->dataItems(team).at(i)->statList().at(18).toInt();
        if(qAbs(enemyRow-row)<=range && qAbs(enemyCol-col)<=range &&
                (qAbs(enemyRow-row)+qAbs(enemyCol-col)<range*2)){
            enemy.append(m_dataSource->dataItems(team).at(i)->statList().at(0).toString());
        }
    }
    return enemy;
}

void GameLogic::removeTroop(bool team, short index)
{
    m_dataSource->removeTroop(team,index);

    emit dataChanged();
}

void GameLogic::addTroop(bool aTeam,short index)
{
    Troop *troop = new Troop;
    short sRow;
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
    QJsonValue attacked(2) ;
    QJsonValue active(true);
    QJsonValue hp(0);
    QJsonValue blank3(0);

    troop->setStatList(0,name);
    troop->setStatList(1,type);
    troop->setStatList(2,maxHp);
    troop->setStatList(3,aVal);
    troop->setStatList(4,def);
    troop->setStatList(5,dDice);
    troop->setStatList(6,dmg);
    troop->setStatList(7,speed);
    troop->setStatList(8,range);
    troop->setStatList(9,aCharge);
    troop->setStatList(10,aCrowd);
    troop->setStatList(11,morale);
    troop->setStatList(12,aType);
    troop->setStatList(13,str);
    troop->setStatList(14,agi);
    troop->setStatList(15,end);
    troop->setStatList(16,will);
    troop->setStatList(17,row);
    troop->setStatList(18,col);
    troop->setStatList(19,id);
    troop->setStatList(20,team);
    troop->setStatList(21,moved);
    troop->setStatList(22,attacked);
    troop->setStatList(23,active);
    troop->setStatList(24,hp);
    troop->setStatList(25,blank3);
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
    for(int i=0;i<m_dataSource->dataItems(team).size();i++){
        QJsonValue hp= m_dataSource->dataItems(team).at(i)->statList().at(2);
        m_dataSource->dataItems(team).at(i)->setStatList(24,hp);
        m_dataSource->dataItems(team).at(i)->setStatList(23,troopActive);
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

