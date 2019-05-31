#pragma once

#include <main_widget/control_panel/control_panel_widget_fwd.hpp>
#include <main_widget/control_panel/relay_widget/relay_widget_fwd.hpp>
#include <main_window/main_window_fwd.hpp>

#include <common/common_type.hpp>
#include <common/relay_command.hpp>
#include <common/signals2.hpp>

namespace barmaley::gui {

class ControlPanelView
{
public:
    using GroupCommandSignalType  = lib::s2::signal<void(lib::DeviceId, lib::GroupCommand)>;
    using SingleCommandSignalType = lib::s2::signal<void(lib::DeviceId, lib::RelayId, lib::SingleCommand)>;

    friend ControlPanelWidget;
    friend RelayWidget;
    friend MainWindow;

    ControlPanelView()                        = default;
    ControlPanelView(const ControlPanelView&) = delete;
    ControlPanelView(ControlPanelView&&)      = delete;

    lib::s2::connection onGroupCommand(GroupCommandSignalType::slot_type slot);
    lib::s2::connection onSingleCommand(SingleCommandSignalType::slot_type slot);

protected:
    GroupCommandSignalType groupEvent;
    SingleCommandSignalType singleEvent;
};
} // namespace barmaley::gui
