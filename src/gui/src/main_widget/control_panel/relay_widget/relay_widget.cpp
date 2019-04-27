#include "relay_widget.hpp"

#include <main_widget/control_panel/control_panel_view.hpp>

#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>

namespace barmaley::gui {
namespace {
QString toQString(lib::RelayStatus rs)
{
    switch (rs) {
        case lib::RelayStatus::OFF:
            return QStringLiteral("Off");
        case lib::RelayStatus::ON:
            return QStringLiteral("On");
        default:
            return QStringLiteral("Invalid");
    }
}
} // namespace
RelayWidget::RelayWidget(ControlPanelView& controlPanelView, QWidget* parent)
    : controlPanelView(controlPanelView), name(new QLabel(this)), status(new QLabel(this))
{
    setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Maximum);
    setObjectName("RelayWidget");
    setStyleSheet("#RelayWidget{background: #fff; border: 1px solid gray;}");
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setAlignment(Qt::AlignTop);
    mainLayout->setContentsMargins(5, 5, 5, 5);
    setLayout(mainLayout);

    mainLayout->addWidget(name);
    mainLayout->addWidget(status);

    auto buttonLayout = new QHBoxLayout(this);
    buttonLayout->setContentsMargins(0, 0, 0, 0);

    auto btnOn  = new QPushButton("On", this);
    auto btnOff = new QPushButton("Off", this);
    auto btnInv = new QPushButton("Inv", this);

    connect(btnOn, &QPushButton::clicked, this, [this] {
        if (!deviceId || !id) {
            return;
        }
        this->controlPanelView.singleEvent(*deviceId, *id, lib::SingleCommand::on);
    });

    connect(btnOff, &QPushButton::clicked, this, [this] {
        if (!deviceId || !id) {
            return;
        }
        this->controlPanelView.singleEvent(*deviceId, *id, lib::SingleCommand::off);
    });

    connect(btnInv, &QPushButton::clicked, this, [this] {
        if (!deviceId || !id) {
            return;
        }
        this->controlPanelView.singleEvent(*deviceId, *id, lib::SingleCommand::inv);
    });

    buttonLayout->addWidget(btnOn);
    buttonLayout->addWidget(btnOff);
    buttonLayout->addWidget(btnInv);

    mainLayout->addLayout(buttonLayout);
}
void RelayWidget::setData(lib::DeviceId deviceId, const lib::Relay& r)
{
    this->deviceId = deviceId;

    id = r.id;
    name->setText(QString::fromStdString(r.name));
    name->setAlignment(Qt::AlignCenter);
    status->setText("Status: " + toQString(r.status));
}
} // namespace barmaley::gui
