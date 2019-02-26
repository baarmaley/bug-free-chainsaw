#pragma once

#include <QWidget>
class QTableView;

namespace barmaley::gui {
class Model;
class MainWidget : public QWidget
{
public:
    MainWidget(QWidget* parent = nullptr);
    ~MainWidget();

    void setModel(Model* model);

private:
    QTableView* tableView;
};
} // namespace barmaley::gui
