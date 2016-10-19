#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include<QDebug>
#include <QtCore>
#include<QQuickView>
#include <QQmlContext>
#include<pasvcontrl.h>
#include<robot.h>
int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    QQmlApplicationEngine engine;
    Robot Rt;
    engine.rootContext()->setContextProperty("robot", &Rt);

    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    return app.exec();
}
