#include "control_panel_view.hpp"
namespace barmaley::gui {
lib::s2::connection ControlPanelView::onGroupCommand(GroupCommandSignalType::slot_type slot)
{
    return groupEvent.connect(std::move(slot));
}
lib::s2::connection ControlPanelView::onSingleCommand(SingleCommandSignalType::slot_type slot)
{
    return singleEvent.connect(std::move(slot));
}
} // namespace barmaley::gui
