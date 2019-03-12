#include <QApplication>

#include <facade/facade.hpp>
#include <main_widget/main_widget.hpp>
#include <main_widget/model/journal_model.hpp>
#include <main_widget/model/model.hpp>

using namespace barmaley;

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);

    lib::Facade facade;
    gui::Model model(facade.currentStateModelView());
    gui::JournalModel journalModel(facade.journalManagerView());

    gui::MainWidget mainWidget;
    mainWidget.setModel(&model, &journalModel);

    mainWidget.show();
    return app.exec();
}
