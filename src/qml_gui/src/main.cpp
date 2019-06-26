#include <QApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

#include <QDebug>

#include <facade/facade.hpp>
#include <model/model.hpp>


using namespace barmaley;

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);
    QApplication::setApplicationName("ESP Watcher");

    lib::Facade facade;
    gui::DeviceList deviceList(facade.currentStateModelView());


    QQmlApplicationEngine engine;
    auto ctxt = engine.rootContext();
    ctxt->setContextProperty("deviceListModel", &deviceList);

    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    return app.exec();
}
