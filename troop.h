#ifndef TROOP_H
#define TROOP_H

#include <QObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QVariant>

class Troop : public QObject
{
    Q_OBJECT

public:
    explicit Troop(QObject *parent = nullptr);

//    int m_id;
//    QString m_name;
//    int m_str;
//    int m_agi;
//    int m_speed;
//    int m_row;
//    int m_col;
//    bool m_team;
//    QString m_aType;
//    QString m_type;
//    int m_end ;
//    int m_hp;
//    int m_aVal;
//    int m_dmg;
//    int m_def;
//    int m_dDice;
//    int m_morale;
//    int m_aCharge;
//    int m_aCrowd;
//    int m_will;
//    int m_range;
//    int m_moved;

    Q_INVOKABLE QJsonArray statList() const;
    void setStatList(int index,QJsonValue &value);



    //saving to JSON
  //  void read();
    void write(QJsonObject &json) const;
//    int id() const;
//    void setId(int id);

//    int row() const;
//    void setRow(int row);

//    int col() const;
//    void setCol(int col);

signals:

    void statListChanged();

private:

    QJsonArray *m_statList;
};

#endif // TROOP_H
