#include <QApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QQuickStyle>

#include <QDebug>

#include <facade/facade.hpp>
#include <model/device_item.hpp>
#include <model/device_list.hpp>
#include <command_helper/command_helper.hpp>

using namespace barmaley;

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);
    QApplication::setApplicationName("ESP Watcher");

    lib::Facade facade;
    gui::DeviceList deviceList(facade.currentStateModelView());
    gui::DeviceItem deviceItem(facade.currentStateModelView());
    gui::CommandHelper commandHelper(facade);

    QQmlApplicationEngine engine;

    QQuickStyle::setStyle("Material");

    auto ctxt = engine.rootContext();
    ctxt->setContextProperty("deviceListModel", &deviceList);
    ctxt->setContextProperty("deviceItem", &deviceItem);

    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    
    auto rootObjects =  engine.rootObjects();
    if (!rootObjects.isEmpty()) {
        auto stackView = rootObjects[0]->findChild<QObject*>("stackView");
        if (stackView) {
            QObject::connect(stackView, SIGNAL(clickedRelay(int, int)), &commandHelper, SLOT(singleCommand(int, int)));
        }
        else {
            qWarning() << "StackView does not found.";
        }
    }
    //auto stackView = rootObject->findChild<QObject*>("mainStack");
    //qDebug() << rootObject->children();
    //for (const auto& item : rootObjects) {
    //    qDebug() << item << item->objectName();
    //}

    return app.exec();
}
