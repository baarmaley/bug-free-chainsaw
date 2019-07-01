#include "command_helper.hpp"

#include <current_state_model/current_state_model.hpp>
#include <facade/facade.hpp>

#include <QDebug>

namespace barmaley::gui {
CommandHelper::CommandHelper(lib::Facade& facade) : facade(facade) {}

void CommandHelper::singleCommand(int device_id, int relay_id)
{
    qDebug() << "singleCommand device_id:" << device_id << "relay_id:" << relay_id;

    auto deviceId = lib::toDeviceId(device_id);
    auto relayId  = lib::toRelayId(relay_id);

    const auto& cModel = facade.currentStateModel();

    auto device = cModel.value(deviceId);
    if (device) {
        const auto& relays = device->status.relays;
        auto it = std::find_if(relays.begin(), relays.end(), [&](const lib::Relay& v) { return v.id == relayId; });
        if (it != relays.end()) {
            facade.singleCommand(deviceId, relayId, lib::SingleCommand::inv);
            return;
        }
        qWarning() << "RelayId:" << relay_id << "in DeviceId:" << device_id << "does not found";
        return;
    }
    qWarning() << "DeviceId:" << device_id << "does not found";
}
} // namespace barmaley::gui
