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

void GameLogic::attack(int attacker, QString attacked,bool team)
{
    short attacks = m_dataSource->dataItems(team).at(attacker)->statList().at(22).toInt();     //pobranie liczby ataków atakującego
    if(attacks==0) {
        m_dataSource->writeConsole("brak możliwości ataku");
        return;
    }
    short attackValue = m_dataSource->dataItems(team).at(attacker)->statList().at(3).toInt();       //pobieranie wartości ataku
    short dmg = m_dataSource->dataItems(team).at(attacker)->statList().at(6).toInt();               //pobieranie obrażeń
    short dmgDice = m_dataSource->dataItems(team).at(attacker)->statList().at(5).toInt();           //pobieranie kości obrażeń
    QString attackerName=m_dataSource->dataItems(team).at(attacker)->statList().at(0).toString();   //pobieranie nazwy atakującego
    QJsonValue json(attacks-1);                                                                     //zmniejszenie liczby ataków atakującego
    m_dataSource->dataItems(team).at(attacker)->setStatList(22,json);                               //zmiana liczby ataków atakującego w datasource
    //atak:
    for(int i =0;i<m_dataSource->dataItems(!team).size();i++){                                      //szukanie przeciwnika
        if(m_dataSource->dataItems(!team).at(i)->statList().at(0).toString()==attacked){

            short defence = m_dataSource->dataItems(!team).at(i)->statList().at(4).toInt();
            short attackRoll=QRandomGenerator::global()->bounded(1,20);
            short attackTest = attackRoll +attackValue - defence;
            if(attackTest>=0) {
                 m_dataSource->writeConsole(attackerName+ " trafia!: " + QString::number(attackRoll) +"+" +QString::number(attackValue)+"="+QString::number(attackRoll+attackValue)+ " vs: " +QString::number(defence));
                short dmgDealt = QRandomGenerator::global()->bounded(1,dmgDice)+dmg;
                m_dataSource->writeConsole("zadano "+QString::number(dmgDealt)+" obrażeń");
                short hp= m_dataSource->dataItems(!team).at(i)->statList().at(2).toInt();
                if(hp<=dmgDealt){
                    m_dataSource->writeConsole(attacked+" zniszczona!");
                    QJsonValue jsonAct(false);
                    m_dataSource->dataItems(!team).at(i)->setStatList(23,jsonAct);
                    emit dataChanged();
                }
                QJsonValue jsonDmg(hp-dmgDealt);
                m_dataSource->dataItems(!team).at(i)->setStatList(2,jsonDmg);
                emit dataChanged();
                return;
            }else {
                m_dataSource->writeConsole(attackerName+ " pudłuje: " + QString::number(attackRoll) +"+" +QString::number(attackValue)+"="+QString::number(attackRoll+attackValue)+ " vs: " +QString::number(defence));
                qInfo() << "pudło";
            }
        }
    }
}



void GameLogic::setDataSource(DataSource *ds)
{
    m_dataSource=ds;
}

void GameLogic::resetMoves()
{
    QJsonValue json(0);
    for(int i=0;i<m_dataSource->dataItems(true).size();i++){
        m_dataSource->dataItems(true).at(i)->setStatList(21,json);
    }
    for(int i=0;i<m_dataSource->dataItems(false).size();i++){
        m_dataSource->dataItems(false).at(i)->setStatList(21,json);
    }

    emit dataChanged();
}
