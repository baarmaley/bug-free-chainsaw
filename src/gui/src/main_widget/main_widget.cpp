#include <main_widget/main_widget.hpp>

#include <main_widget/control_panel/control_panel_widget.hpp>
#include <main_widget/model/journal_model.hpp>
#include <main_widget/model/model.hpp>
#include <main_widget/model/model_list.hpp>

#include <QHBoxLayout>
#include <QLineEdit>
#include <QListView>
#include <QPushButton>
#include <QStackedWidget>
#include <QTableView>
#include <QVBoxLayout>

// debug
#include <QDebug>

namespace barmaley::gui {
MainWidget::MainWidget(QWidget* parent)
    : QWidget(parent),
      currentStateTableView(new QTableView(this)),
      listView(new QListView(this)),
      journalTableView(new QTableView(this)),
      controlPanelWidget(new ControlPanelWidget(this))
{
    setWindowTitle(tr("Esp Watcher"));

    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    setLayout(mainLayout);

    auto navigationLayout = new QHBoxLayout(this);
    auto debugBtn         = new QPushButton("Debug", this);
    auto listBtn          = new QPushButton("List", this);
    auto returnBtn        = new QPushButton("Return", this);
    navigationLayout->addWidget(debugBtn);
    navigationLayout->addWidget(listBtn);
    navigationLayout->addWidget(returnBtn);
    mainLayout->addLayout(navigationLayout);

    auto stackWidget = new QStackedWidget(this);
    mainLayout->addWidget(stackWidget);

    auto debugPage = new QWidget(this);

    auto debugPageLayout = new QVBoxLayout(debugPage);
    debugPageLayout->setContentsMargins(0, 0, 0, 0);
    debugPageLayout->setSpacing(0);
    debugPageLayout->addWidget(currentStateTableView);
    debugPageLayout->addWidget(journalTableView);

    auto listViewPage       = new QWidget(this);
    auto listViewPageLayout = new QVBoxLayout(listViewPage);
    listViewPageLayout->setContentsMargins(0, 0, 0, 0);
    listViewPageLayout->setSpacing(0);
    listViewPageLayout->addWidget(listView);

    auto controlPanelPage       = new QWidget(this);
    auto controlPanelPageLayout = new QVBoxLayout(controlPanelPage);

    controlPanelPageLayout->addWidget(controlPanelWidget);

    controlPanelPageLayout->setContentsMargins(0, 0, 0, 0);
    controlPanelPageLayout->setSpacing(0);

    connect(debugBtn,
            &QPushButton::clicked,
            stackWidget,
            std::bind(&QStackedWidget::setCurrentWidget, stackWidget, debugPage));
    connect(listBtn,
            &QPushButton::clicked,
            stackWidget,
            std::bind(&QStackedWidget::setCurrentWidget, stackWidget, listViewPage));

    connect(returnBtn,
            &QPushButton::clicked,
            stackWidget,
            std::bind(&QStackedWidget::setCurrentWidget, stackWidget, listViewPage));

    connect(stackWidget, &QStackedWidget::currentChanged, returnBtn, [returnBtn, stackWidget, controlPanelPage](int) {
        returnBtn->setVisible(stackWidget->currentWidget() == controlPanelPage);
    });

    connect(listView, &QListView::clicked, [this, stackWidget, controlPanelPage](const QModelIndex& index) {
        auto id = static_cast<ModelList*>(this->listView->model())->deviceId(index.row());
        controlPanelWidget->setCurrentDevice(id);
        stackWidget->setCurrentWidget(controlPanelPage);
    });

    stackWidget->addWidget(debugPage);
    stackWidget->addWidget(listViewPage);
    stackWidget->addWidget(controlPanelPage);

    stackWidget->setCurrentWidget(listViewPage);
}
MainWidget::~MainWidget() = default;

void MainWidget::setModel(lib::CurrentStateModelView* currentStateModelView,
                          Model* model,
                          ModelList* modelList,
                          JournalModel* journalModel)
{
    currentStateTableView->setModel(model);
    listView->setModel(modelList);
    journalTableView->setModel(journalModel);
    controlPanelWidget->setModel(*currentStateModelView);
}
ControlPanelView* MainWidget::controlPanelView()
{
    return controlPanelWidget->controlPanelView();
}
} // namespace barmaley::gui
