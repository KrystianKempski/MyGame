#include "appwrapper.h"

AppWrapper::AppWrapper(QObject *parent,QGuiApplication *app) : QObject(parent)
{
    m_app=app;
}
void AppWrapper::initialize()
{
    DataSource * ds = new DataSource(this);
  //  BattlefieldModel2 bat;
 //   Database ds2;
     m_battleModel.setDataSource(ds);
     m_troopModelBlue.setDataSource(ds);
    m_troopModelRed.setDataSource(ds);
    m_gameLogic.setDataSource(ds);


    connect(&m_troopModelRed,&TableModel2::troopChaged,ds,&DataSource::changeItem);
    connect(&m_troopModelBlue,&TableModelBlue::troopChaged,ds,&DataSource::changeItem);
    connect(ds,&DataSource::troopChanged,&m_battleModel,&BattlefieldModel2::updateModel);
    connect(&m_gameLogic,&GameLogic::dataChanged,&m_troopModelRed,&TableModel2::updateAll);
    connect(&m_gameLogic,&GameLogic::dataChanged,&m_troopModelBlue,&TableModelBlue::updateModel);
    connect(ds,&DataSource::troopChanged,&m_troopModelRed,&TableModel2::updateModel);
    connect(ds,&DataSource::troopChanged,&m_troopModelBlue,&TableModelBlue::updateModel);

    m_engine.rootContext()->setContextProperty("troopModelBlue",&m_troopModelBlue);
    m_engine.rootContext()->setContextProperty("troopModelRed",&m_troopModelRed);
     m_engine.rootContext()->setContextProperty("gamelogic",&m_gameLogic);
    m_engine.rootContext()->setContextProperty("battleModel2", &m_battleModel);
    m_engine.rootContext()->setContextProperty("dataSource",ds);
    m_app->setOrganizationName("KrystianKempski");
    m_app->setOrganizationDomain("KrystianKempski");

    const QUrl url(QStringLiteral("qrc:/main.qml"));
    connect(&m_engine, &QQmlApplicationEngine::objectCreated,
            m_app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    m_engine.load(url);
    //connect(m_app,&QGuiApplication::aboutToQuit,[=](){qInfo()<<"test";} );


}
