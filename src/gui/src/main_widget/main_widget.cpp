#include <main_widget/main_widget.hpp>

#include <main_widget/model/journal_model.hpp>
#include <main_widget/model/model.hpp>

#include <QLineEdit>
#include <QTableView>
#include <QVBoxLayout>

namespace barmaley::gui {
MainWidget::MainWidget(QWidget* parent)
    : QWidget(parent), currentStateTableView(new QTableView(this)), journalTableView(new QTableView(this))
{
    QLineEdit* lineEdit     = new QLineEdit(this);
    QVBoxLayout* mainLayout = new QVBoxLayout(this);

    mainLayout->addWidget(lineEdit);
    mainLayout->addWidget(currentStateTableView);
    mainLayout->addWidget(journalTableView);
    setLayout(mainLayout);

    setWindowTitle(tr("Esp Watcher"));
}
MainWidget::~MainWidget() = default;

void MainWidget::setModel(Model* model, JournalModel* journalModel)
{
    currentStateTableView->setModel(model);
    journalTableView->setModel(journalModel);
}
} // namespace barmaley::gui
