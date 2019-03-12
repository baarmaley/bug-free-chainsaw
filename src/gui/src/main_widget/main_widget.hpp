#pragma once

#include <main_widget/model/journal_model_fwd.hpp>
#include <main_widget/model/model.hpp>

#include <QWidget>

class QTableView;

namespace barmaley::gui {

class MainWidget : public QWidget
{
public:
    MainWidget(QWidget* parent = nullptr);
    ~MainWidget();

    void setModel(Model* model, JournalModel* journalModel);

private:
    QTableView* currentStateTableView;
    QTableView* journalTableView;
};
} // namespace barmaley::gui
