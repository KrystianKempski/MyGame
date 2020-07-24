#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <qqml.h>
#include <QQmlContext>
#include <QAbstractTableModel>
#include "datasource.h"
#include "battlefieldmodel2.h"
#include "troopmodelred.h"
#include "appwrapper.h"

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);


     AppWrapper wrapper(nullptr,&app);
     wrapper.initialize();

     return app.exec();
}
