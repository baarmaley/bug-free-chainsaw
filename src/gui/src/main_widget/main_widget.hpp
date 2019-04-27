#pragma once

#include <main_widget/control_panel/control_panel_view_fwd.hpp>
#include <main_widget/control_panel/control_panel_widget_fwd.hpp>
#include <main_widget/model/journal_model_fwd.hpp>
#include <main_widget/model/model_fwd.hpp>
#include <main_widget/model/model_list_fwd.hpp>

#include <current_state_model/current_state_model_fwd.hpp>

#include <QWidget>

class QTableView;
class QListView;

namespace barmaley::gui {

class MainWidget : public QWidget
{
public:
    MainWidget(QWidget* parent = nullptr);
    ~MainWidget();

    void setModel(lib::CurrentStateModelView* currentStateModelView,
                  Model* model,
                  ModelList* modelList,
                  JournalModel* journalModel);

    ControlPanelView* controlPanelView();

private:
    QTableView* currentStateTableView;
    QListView* listView;
    QTableView* journalTableView;
    ControlPanelWidget* controlPanelWidget;
};
} // namespace barmaley::gui
