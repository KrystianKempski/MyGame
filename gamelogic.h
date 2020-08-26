/****************************************************************************
** Klasa GameLogic przechowuje metody pozwalające na obsługę i sterowanie jed-
** nostkami na polu bitwy. Kontroluje też zasady potyczki, dodawanie i usuwanie
** jednostek oraz turę gry.
****************************************************************************/

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
    Q_INVOKABLE void attack(quint8 attackerIndex,const QString& defenderName,bool team);
    void setDataSource(DataSource* ds);
    Q_INVOKABLE void resetTroop(quint8 troopIndex, bool team);
    Q_INVOKABLE QStringList findEnemy(quint8 row, quint8 col, quint8 range, bool team) const;
    Q_INVOKABLE void removeTroop(bool team, quint8 index);
    Q_INVOKABLE void addTroop(bool team, quint8 index);
    Q_INVOKABLE void startNewGame();
    Q_INVOKABLE void resetAllTroops(bool team);
    Q_INVOKABLE void removeAllTroops(bool team);
    Q_INVOKABLE void waitForTurn(bool team);
    Q_INVOKABLE void resetTroopsPosition(bool team);
    void update();

signals:
    void dataChanged();
private:
    void resetMoves();
    DataSource* m_dataSource;
//    quint8 m_turn;
    bool m_teamTurn;
    QTimer m_timer;


};

#endif // GAMELOGIC_H
