#pragma once

#include <common/common_type.hpp>
#include <common/signals2.hpp>

#include <current_state_model/current_state_model_fwd.hpp>
#include <current_state_model/current_state_model_view_fwd.hpp>

#include <main_widget/control_panel/control_panel_view_fwd.hpp>
#include <main_widget/control_panel/relay_widget/relay_widget_fwd.hpp>

#include <QWidget>

#include <optional>
#include <vector>

class QLabel;
class QVBoxLayout;

namespace barmaley::gui {
class ControlPanelWidget : public QWidget
{
public:
    ControlPanelWidget(QWidget* parent = nullptr);

    void setModel(lib::CurrentStateModelView& modelView);

    void setCurrentDevice(lib::DeviceId id);

    inline ControlPanelView* controlPanelView()
    {
        return controlPanelView_.get();
    }

private:
    std::unique_ptr<ControlPanelView> controlPanelView_;
    const lib::CurrentStateModel* currentStateModel = nullptr;
    std::optional<lib::DeviceId> currentDeviceId;
    QLabel* nameDevice          = nullptr;
    QVBoxLayout* relayContainer = nullptr;

    std::vector<RelayWidget*> relayWidgets;

    lib::ConnectionsContainer connectionsContainer;
};
} // namespace barmaley::gui
