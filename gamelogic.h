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
    Q_INVOKABLE void endTurn(const bool &team);
    Q_INVOKABLE void attack(const short& attackerIndex,const QString &defenderName, const bool & team);
    void setDataSource(DataSource* ds);
    Q_INVOKABLE void resetTroop(const short& troopIndex,const bool& team);
    Q_INVOKABLE QStringList findEnemy(const short& row,const short& col,const short &range,const bool &team) const;
    Q_INVOKABLE void removeTroop(const bool& team,const short& index);
    Q_INVOKABLE void addTroop(const bool & team,const short& index);
    Q_INVOKABLE void startNewGame();
    Q_INVOKABLE void resetAllTroops(const bool& team);
    Q_INVOKABLE void removeAllTroops(const bool& team);
    Q_INVOKABLE void waitForTurn(const bool& team);
    Q_INVOKABLE void resetTroopsPosition(const bool& team);
    void update();

signals:
    void dataChanged();
private:
    void resetMoves();
    DataSource* m_dataSource;
    bool m_teamTurn;
    QTimer m_timer;


};

#endif // GAMELOGIC_H
