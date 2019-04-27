#pragma once

#include <main_widget/control_panel/control_panel_view_fwd.hpp>

#include <common/common_type.hpp>

#include <QWidget>

class QLabel;

namespace barmaley::gui {
class RelayWidget : public QWidget
{
public:
    RelayWidget(ControlPanelView& controlPanelView, QWidget* parent = nullptr);

    void setData(lib::DeviceId deviceId, const lib::Relay& r);

private:
    ControlPanelView& controlPanelView;
    QLabel* name;
    QLabel* status;

    std::optional<lib::DeviceId> deviceId;
    std::optional<lib::RelayId> id;
};
} // namespace barmaley::gui
