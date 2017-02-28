#include <QApplication>
#include "widget.h"
#include "WinsockMatTransmissionClient.h"
int main(int argc, char *argv[])
{
    //QGuiApplication app(argc, argv);

    //QQmlApplicationEngine engine;
    //engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    QApplication app(argc, argv);
    Widget w;
    w.show();
    app.exit(0);

    return app.exec();
}

