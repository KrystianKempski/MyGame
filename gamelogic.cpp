#include "gamelogic.h"

GameLogic::GameLogic(QObject *parent) : QObject(parent)
{

}

void GameLogic::startGame()
{
    //m_turn=0;
    resetMoves();

}

void GameLogic::playerReady(bool team)
{
    team? m_teamRedReady=true:m_teamBlueReady=true;
}

void GameLogic::nextTurn()
{
    if(m_teamRedReady&&m_teamBlueReady){
        m_turn++;
        m_teamRedReady=false;
        m_teamBlueReady=true;
        resetMoves();
    }else{
        m_teamRedReady? qInfo()<< "team blue is not ready" : qInfo() << "team red is not ready";
    }
}

void GameLogic::attack(int attackerIndex, QString defenderName,bool team)
{
    QString message="";
    short attacks = m_dataSource->dataItems(team).at(attackerIndex)->statList().at(22).toInt();     //pobranie liczby ataków atakującego
    if(attacks==0) {
        message=message+"brak możliwości ataku";
    }else{
        short attackValue = m_dataSource->dataItems(team).at(attackerIndex)->statList().at(3).toInt();       //pobieranie wartości ataku
        short dmg = m_dataSource->dataItems(team).at(attackerIndex)->statList().at(6).toInt();               //pobieranie obrażeń
        short dmgDice = m_dataSource->dataItems(team).at(attackerIndex)->statList().at(5).toInt();           //pobieranie kości obrażeń
        QString attackerName=m_dataSource->dataItems(team).at(attackerIndex)->statList().at(0).toString();   //pobieranie nazwy atakującego
        QJsonValue json(attacks-1);                                                                     //zmniejszenie liczby ataków atakującego
        m_dataSource->dataItems(team).at(attackerIndex)->setStatList(22,json);                               //zmiana liczby ataków atakującego w datasource
        //atak:
        for(int i =0;i<m_dataSource->dataItems(!team).size();i++){                                      //szukanie przeciwnika
            if(m_dataSource->dataItems(!team).at(i)->statList().at(0).toString()==defenderName){
                short defence = m_dataSource->dataItems(!team).at(i)->statList().at(4).toInt();
                short attackRoll=QRandomGenerator::global()->bounded(1,20);
                short attackTest = attackRoll +attackValue - defence;
                if(attackTest>=0) {
                    message=message+attackerName+ " trafia "+ defenderName+"!: " + QString::number(attackRoll) +"+" +QString::number(attackValue)+"="+QString::number(attackRoll+attackValue)+ " vs: " +QString::number(defence);
                    short dmgDealt = QRandomGenerator::global()->bounded(1,dmgDice)+dmg;
                    message=message+"\r\n zadano "+QString::number(dmgDealt)+" obrażeń ";
                    short hp= m_dataSource->dataItems(!team).at(i)->statList().at(2).toInt();
                    if(hp<=dmgDealt){
                        message=message+"\r\n"+defenderName+" zniszczona!";
                        QJsonValue troopActive(false);
                        m_dataSource->dataItems(!team).at(i)->setStatList(23,troopActive);                  //zmiana statusu jednostki na nieaktywny
                     //   emit dataChanged();
                    }
                    QJsonValue jsonDmg(hp-dmgDealt);
                    m_dataSource->dataItems(!team).at(i)->setStatList(2,jsonDmg);                       //zmiana hp jednostyki
                   // emit dataChanged();
                    // return;
                }else {
                    message=message+attackerName+ " pudłuje "+defenderName+": " + QString::number(attackRoll) +"+" +QString::number(attackValue)+"="+QString::number(attackRoll+attackValue)+ " vs: " +QString::number(defence);
                }
            }
        }
    }
    m_dataSource->writeConsole(message);
    emit dataChanged();
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
