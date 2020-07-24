#ifndef APPWRAPPER_H
#define APPWRAPPER_H

#include <QObject>
#include <QQmlApplicationEngine>
#include <QGuiApplication>
#include <QQmlContext>
#include <QQmlApplicationEngine>
#include "troopmodelred.h"
#include "troopmodelblue.h"
#include "battlefieldmodel2.h"
#include "datasource.h"
#include "gamelogic.h"
//#include "database.h"

class AppWrapper : public QObject
{
    Q_OBJECT
public:
    explicit AppWrapper(QObject *parent = nullptr, QGuiApplication * app=nullptr);
    void initialize();

signals:
private:
        QQmlApplicationEngine m_engine;
        TableModel2 m_troopModelRed;
        TableModelBlue m_troopModelBlue;
        BattlefieldModel2 m_battleModel;
        GameLogic m_gameLogic;
        QGuiApplication * m_app;
};

#endif // APPWRAPPER_H
