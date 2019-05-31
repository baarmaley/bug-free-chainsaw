#include "main_window.hpp"

#include <button/button.hpp>
#include <current_state_model/current_state_model.hpp>
#include <main_widget/control_panel/control_panel_view.hpp>

#include <QLabel>
#include <QVBoxLayout>

#include <QDebug>

namespace barmaley::gui {
namespace {
Button::StateType buttonState(lib::RelayStatus v)
{
    switch (v) {
        case lib::RelayStatus::ON:
            return Button::StateType::on;
        case lib::RelayStatus::OFF:
            return Button::StateType::off;
        default:
            return Button::StateType::notAvailable;
    }
}
} // namespace

MainWindow::MainWindow(QWidget* parent)
    : QWidget(parent),
      logo(":/logo.png"),
      controlPanelView_(std::make_unique<ControlPanelView>()),
      deviceName(new QLabel("UnknownName", this))
{
    setObjectName("MainWindow");
    setStyleSheet(R"(
        #MainWindow{ 
            min-width: 248px;
            max-width: 248px;
            border-image: url(:/background.jpg) 0 0 0 0 stretch stretch;
        }
        #NameLabel{
            color: white;
            font-size: 20px;
            font-weight: 500;
        })");

    setWindowFlags(Qt::MSWindowsFixedSizeDialogHint);

    auto mainLayout = new QVBoxLayout(this);
    setLayout(mainLayout);
    mainLayout->setContentsMargins(0, 10, 0, 10);

    auto topLayout = new QVBoxLayout(this);
    topLayout->setSpacing(0);
    topLayout->setAlignment(Qt::AlignTop);
    topLayout->setContentsMargins(0, 0, 0, 20);
    mainLayout->addLayout(topLayout);

    auto logoLabel = new QLabel(this);
    logoLabel->setObjectName("LogoLabel");
    logoLabel->setPixmap(logo);
    topLayout->addWidget(logoLabel);
    topLayout->setAlignment(logoLabel, Qt::AlignHCenter);

    deviceName->setObjectName("NameLabel");
    topLayout->addWidget(deviceName);
    topLayout->setAlignment(deviceName, Qt::AlignHCenter);

    buttonContainer = new QVBoxLayout(this);
    buttonContainer->setSpacing(10);
    buttonContainer->setContentsMargins(10, 0, 10, 20);
    buttonContainer->setAlignment(Qt::AlignTop);
    mainLayout->addLayout(buttonContainer);

    mainLayout->setStretchFactor(topLayout, 1);
}

void MainWindow::setModel(lib::CurrentStateModelView& modelView)
{
    currentStateModel = &modelView.currentStateModel;

    auto filter = [this](lib::DeviceId key) {
        if (!currentDeviceId || *currentDeviceId != key) {
            return true;
        }
        return false;
    };

    connectionsContainer += modelView.onRelayVectorChanged([this, filter](lib::DeviceId key) {
        if (filter(key)) {
            return;
        }
        setCurrentDevice(*currentDeviceId);
    });
    connectionsContainer += modelView.onAdded([this, filter](lib::DeviceId key) {
        if (filter(key)) {
            return;
        }
        setCurrentDevice(*currentDeviceId);
    });

    connectionsContainer += modelView.onBusyChanged([this, filter](lib::DeviceId key) {
        if (filter(key)) {
            return;
        }
        setCurrentDevice(*currentDeviceId);
    });
}

void MainWindow::setCurrentDevice(lib::DeviceId id)
{
    currentDeviceId = id;
    if (!currentStateModel) {
        return;
    }
    auto value = currentStateModel->value(id);
    if (!value) {
        return;
    }
    deviceName->setText(QString::fromStdString(value->status.name));

    const auto& relays = value->status.relays;
    for (std::size_t i = 0; i < relays.size(); ++i) {
        const auto& cRelay = relays[i];

        if (i >= buttons.size()) {
            auto relayWidget = new Button(this);
            buttonContainer->addWidget(relayWidget);
            buttons.emplace_back(cRelay.id, relayWidget);
            connect(buttons.back().widget, &QPushButton::clicked, this, [pos = i, this] {
                controlPanelView_->singleEvent(*currentDeviceId, buttons[pos].id, lib::SingleCommand::inv);
            });
        }

        auto button = buttons[i].widget;
        auto bState = buttonState(cRelay.status);

        button->setState(bState);
        button->setEnabled(!(bState == Button::StateType::notAvailable) && !value->isBusy);
        button->setText(QString::fromStdString(cRelay.name));
    }
    while (buttons.size() > relays.size()) {
        buttons.back().widget->deleteLater();
        buttons.pop_back();
    }
}
} // namespace barmaley::gui
