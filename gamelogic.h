#ifndef GAMELOGIC_H
#define GAMELOGIC_H

#include <QObject>
#include "troop.h"
#include "datasource.h"
#include "troopmodelred.h"
#include <QRandomGenerator>

class GameLogic : public QObject
{
    Q_OBJECT
public:
    explicit GameLogic(QObject *parent = nullptr);
    Q_INVOKABLE void startGame();
    Q_INVOKABLE void playerReady(bool team);
    Q_INVOKABLE void nextTurn();
    Q_INVOKABLE void attack(int attackerIndex, QString defenderName,bool team);
    void setDataSource(DataSource* ds);

signals:
    void dataChanged();
private:
    void resetMoves();
    DataSource* m_dataSource;
    short m_turn;
    bool m_teamRedReady=false;
    bool m_teamBlueReady=true;

};

#endif // GAMELOGIC_H
