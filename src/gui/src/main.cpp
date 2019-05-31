#include <fonts/fonts.hpp>
#include <main_window/main_window.hpp>

#include <facade/facade.hpp>
#include <main_widget/control_panel/control_panel_view.hpp>
#include <main_widget/main_widget.hpp>
#include <main_widget/model/journal_model.hpp>
#include <main_widget/model/model.hpp>
#include <main_widget/model/model_list.hpp>

#include <QApplication>
#include <QTimer>

using namespace barmaley;

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);
    QApplication::setApplicationName("ESP Watcher");

    //==================================================
    loading_fonts_and_set_default_font();

    Q_INIT_RESOURCE(resource);

    gui::MainWindow mainWindow;
    QTimer::singleShot(0, [&] {
        mainWindow.show();
        mainWindow.setCurrentDevice(lib::DeviceId{56666541});
    });

    //==================================================

    lib::Facade facade;
    gui::Model model(facade.currentStateModelView());
    gui::ModelList modelList(facade.currentStateModelView());
    gui::JournalModel journalModel(facade.journalManagerView());

    gui::MainWidget mainWidget;
    mainWidget.setModel(&facade.currentStateModelView(), &model, &modelList, &journalModel);
    mainWindow.setModel(facade.currentStateModelView());

    lib::ConnectionsContainer connectionsContainer;
    connectionsContainer += mainWidget.controlPanelView()->onSingleCommand(
        [&facade](lib::DeviceId id, lib::RelayId relayId, lib::SingleCommand cmd) {
            facade.singleCommand(id, relayId, cmd);
        });
    connectionsContainer += mainWidget.controlPanelView()->onGroupCommand(
        [&facade](lib::DeviceId id, lib::GroupCommand cmd) { facade.groupCommand(id, cmd); });

    connectionsContainer += mainWindow.controlPanelView()->onSingleCommand(
        [&facade](lib::DeviceId id, lib::RelayId relayId, lib::SingleCommand cmd) {
            facade.singleCommand(id, relayId, cmd);
        });

    mainWidget.show();
    return app.exec();
}
