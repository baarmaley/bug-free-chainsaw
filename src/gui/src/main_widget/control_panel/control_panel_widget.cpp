#include "control_panel_widget.hpp"

#include <current_state_model/current_state_model.hpp>
#include <main_widget/control_panel/control_panel_view.hpp>
#include <main_widget/control_panel/relay_widget/relay_widget.hpp>

#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QScrollArea>
#include <QVBoxLayout>

namespace barmaley::gui {
ControlPanelWidget::ControlPanelWidget(QWidget* parent)
    : QWidget(parent), controlPanelView_(std::make_unique<ControlPanelView>()), nameDevice(new QLabel(this))
{
    nameDevice->setAlignment(Qt::AlignCenter);

    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(1, 9, 1, 9);
    mainLayout->setAlignment(Qt::AlignTop);
    setLayout(mainLayout);

    auto buttonLayout  = new QHBoxLayout(this);
    auto allTurnOffBtn = new QPushButton("All turn off", this);
    auto allTurnOnBtn  = new QPushButton("All turn on", this);
    buttonLayout->addWidget(allTurnOffBtn);
    buttonLayout->addWidget(allTurnOnBtn);

    connect(allTurnOnBtn, &QPushButton::clicked, this, [this] {
        if (!currentDeviceId) {
            return;
        }
        controlPanelView_->groupEvent(*currentDeviceId, lib::GroupCommand::on);
    });
    connect(allTurnOffBtn, &QPushButton::clicked, this, [this] {
        if (!currentDeviceId) {
            return;
        }
        controlPanelView_->groupEvent(*currentDeviceId, lib::GroupCommand::off);
    });

    mainLayout->addWidget(nameDevice);
    mainLayout->addLayout(buttonLayout);
    relayContainer = new QVBoxLayout(this);
    mainLayout->addLayout(relayContainer);
}
void ControlPanelWidget::setModel(lib::CurrentStateModelView& modelView)
{
    currentStateModel = &modelView.currentStateModel;

    // connectionsContainer += modelView.onIpChanged([this](lib::DeviceId key) {
    //    if (!currentDeviceId || *currentDeviceId != key) {
    //        return;
    //    }
    //});
    connectionsContainer += modelView.onRelayVectorChanged([this](lib::DeviceId key) {
        if (!currentDeviceId || *currentDeviceId != key) {
            return;
        }
        setCurrentDevice(*currentDeviceId);
    });
}
void ControlPanelWidget::setCurrentDevice(lib::DeviceId id)
{
    currentDeviceId = id;
    if (!currentStateModel) {
        return;
    }
    auto value = currentStateModel->value(id);
    nameDevice->setText(QString::fromStdString(value->status.name));

    const auto& relays = value->status.relays;
    for (std::size_t i = 0; i < relays.size(); ++i) {
        if (i >= relayWidgets.size()) {
            auto relayWidget = new RelayWidget(*controlPanelView_, this);
            relayContainer->addWidget(relayWidget);
            relayWidgets.push_back(relayWidget);
        }
        relayWidgets[i]->setData(*currentDeviceId, relays[i]);
    }
    while (relayWidgets.size() > relays.size()) {
        relayWidgets.back()->deleteLater();
        relayWidgets.pop_back();
    }
}
} // namespace barmaley::gui
