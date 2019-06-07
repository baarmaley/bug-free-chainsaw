#pragma once

#include <common/common_type.hpp>
#include <common/signals2.hpp>

#include <current_state_model/current_state_model_fwd.hpp>
#include <current_state_model/current_state_model_view_fwd.hpp>

#include <button/button_fwd.hpp>
#include <main_widget/control_panel/control_panel_view_fwd.hpp>

#include <QPixmap>
#include <QWidget>

#include <optional>
#include <vector>

class QLabel;
class QVBoxLayout;

namespace barmaley::gui {
class MainWindow : public QWidget
{
    struct ButtonItem
    {
        ButtonItem(lib::RelayId id, Button* widget) : id(id), widget(widget) {}
        lib::RelayId id;
        Button* widget;
    };

public:
    MainWindow(QWidget* parent = nullptr);

    void setModel(lib::CurrentStateModelView& modelView);

    void setCurrentDevice(lib::DeviceId id);

    inline ControlPanelView* controlPanelView()
    {
        return controlPanelView_.get();
    }

private slots:

private:
    QPixmap logo;

    std::unique_ptr<ControlPanelView> controlPanelView_;

    QLabel* deviceNameLabel      = nullptr;
    QLabel* connectionLostLabel  = nullptr;
    QLabel* waitLabel            = nullptr;
    QVBoxLayout* buttonContainer = nullptr;

    const lib::CurrentStateModel* currentStateModel = nullptr;
    std::optional<lib::DeviceId> currentDeviceId;

    std::vector<ButtonItem> buttons;

    lib::ConnectionsContainer connectionsContainer;
};
} // namespace barmaley::gui
