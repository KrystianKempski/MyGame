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
    Q_INVOKABLE void endTurn(bool team);
    Q_INVOKABLE void attack(int attackerIndex, QString defenderName,bool team);
    void setDataSource(DataSource* ds);
    Q_INVOKABLE void resetTroop(short troopIndex, bool team);
    Q_INVOKABLE QStringList findEnemy(int row, int col, int range, bool team) const;

signals:
    void dataChanged();
private:
    void resetMoves();
    DataSource* m_dataSource;
    short m_turn;
    bool m_teamTurn;


};

#endif // GAMELOGIC_H
