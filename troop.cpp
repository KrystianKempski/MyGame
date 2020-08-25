#include "troop.h"
#include <QtDebug>
Troop::Troop(QObject *parent) : QObject(parent)

{
    m_statList = new QJsonArray();
    m_statList->append("");      //0 - ID   id
    m_statList->append("");      //1 - TEAM drużyna
    m_statList->append("");      //2 - NAME nazwa jednostki
    m_statList->append("");      //3 - TYPE  typ jednostki
    m_statList->append("");      //4 - STR siła
    m_statList->append("");      //5 - AGI zręczność
    m_statList->append("");      //6 - END wytrzymałość
    m_statList->append("");      //7 - HP punkty życia
    m_statList->append("");      //8 - A_TYPE  typ ataku
    m_statList->append("");      //9 - A_VAL    wartość ataku
    m_statList->append("");      //10 - DMG obrażenia
    m_statList->append("");      //11 - DEF  obrona
    m_statList->append("");      //12 - A_DICE  kość ataku
    m_statList->append("");      //13 - MORALE morale
    m_statList->append("");      //14 - SPEED szybkość
    m_statList->append("");      //15 - A_CHARGE bonus z szarży
    m_statList->append("");      //16 - A+CROWD atak w ścisku
    m_statList->append("");      //17 - ROW rząd na polu bitwy
    m_statList->append("");      //18 - COL kolumna na polu bitwy
    m_statList->append("");      //19 - WILL wola
    m_statList->append("");      //20 - RANGE zasięg ataku
    m_statList->append("");      //21 - MOVED sprawdza czy token poruszał się w danej turze
    m_statList->append("");      //22 - ATTACKED sprawdza czy token atakował się w danej turze
    m_statList->append("");      //23 - BLANK sprawdza czy token poruszał się w danej turze
    m_statList->append("");      //24 - BLANK2 sprawdza czy token poruszał się w danej turze
    m_statList->append("");      //25 - BLANK3 sprawdza czy token poruszał się w danej turze
}

Troop::~Troop()
{
    delete m_statList;
}


void Troop::write(QJsonObject &json) const
{

    json["NAME"] = m_statList->at(0).toString();
    json["TYPE"] = m_statList->at(1).toString();
    json["MAX_HP"] = m_statList->at(2).toInt();
    json["A_VAL"] = m_statList->at(3).toInt();
    json["DEF"] = m_statList->at(4).toInt();
    json["D_DICE"] = m_statList->at(5).toInt();
    json["DMG"] = m_statList->at(6).toInt();
    json["SPEED"] = m_statList->at(7).toInt();
    json["RANGE"]= m_statList->at(8).toInt() ;
    json["A_CHARGE"] = m_statList->at(9).toInt();
    json["A_CROWD"] = m_statList->at(10).toInt();
    json["MORALE"] = m_statList->at(11).toInt();
    json["A_TYPE"] = m_statList->at(12).toString();
    json["STR"] = m_statList->at(13).toInt() ;
    json["AGI"]= m_statList->at(14).toInt() ;
    json["END"] = m_statList->at(15).toInt();
    json["WILL"]= m_statList->at(16).toInt() ;
    json["ROW"]= m_statList->at(17).toInt() ;
    json["COL"]= m_statList->at(18).toInt() ;
    json["T_ID"] = m_statList->at(19).toInt() ;
    json["TEAM"] = m_statList->at(20).toBool();
    json["MOVED"] = m_statList->at(21).toBool();
    json["ATTACKS"] = m_statList->at(22).toInt();
    json["ACTIVE"] = m_statList->at(23).toBool();
    json["HP"] = m_statList->at(24).toInt();
    json["BLANK3"] = m_statList->at(25).toInt();
}
QJsonArray Troop::statList() const
{
    return *m_statList;
}

template<>
void Troop::setStatList<QJsonValue>(int index, QJsonValue &&value)
{
    m_statList->replace(index,value);
    emit statListChanged();
}
