#include <main_widget/main_widget.hpp>

#include <main_widget/model/model.hpp>

#include <QLineEdit>
#include <QTableView>
#include <QVBoxLayout>

namespace barmaley::gui {
MainWidget::MainWidget(QWidget* parent) : QWidget(parent), tableView(new QTableView(this))
{
    QLineEdit* lineEdit     = new QLineEdit(this);
    QVBoxLayout* mainLayout = new QVBoxLayout(this);

    mainLayout->addWidget(lineEdit);
    mainLayout->addWidget(tableView);
    setLayout(mainLayout);

    setWindowTitle(tr("Esp Watcher"));
}
MainWidget::~MainWidget() = default;

void MainWidget::setModel(Model* model)
{
    tableView->setModel(model);
}
} // namespace barmaley::gui
