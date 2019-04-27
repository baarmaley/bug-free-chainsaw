#include <QApplication>

#include <facade/facade.hpp>
#include <main_widget/control_panel/control_panel_view.hpp>
#include <main_widget/main_widget.hpp>
#include <main_widget/model/journal_model.hpp>
#include <main_widget/model/model.hpp>
#include <main_widget/model/model_list.hpp>

using namespace barmaley;

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);

    lib::Facade facade;
    gui::Model model(facade.currentStateModelView());
    gui::ModelList modelList(facade.currentStateModelView());
    gui::JournalModel journalModel(facade.journalManagerView());

    gui::MainWidget mainWidget;
    mainWidget.setModel(&facade.currentStateModelView(), &model, &modelList, &journalModel);

    lib::ConnectionsContainer connectionsContainer;
    connectionsContainer += mainWidget.controlPanelView()->onSingleCommand(
        [&facade](lib::DeviceId id, lib::RelayId relayId, lib::SingleCommand cmd) {
            facade.singleCommand(id, relayId, cmd);
        });
    connectionsContainer += mainWidget.controlPanelView()->onGroupCommand(
        [&facade](lib::DeviceId id, lib::GroupCommand cmd) { facade.groupCommand(id, cmd); });

    mainWidget.show();
    return app.exec();
}
