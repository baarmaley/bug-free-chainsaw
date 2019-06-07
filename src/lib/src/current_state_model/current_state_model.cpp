#include <current_state_model/current_state_model.hpp>

#include <chrono>

namespace barmaley::lib {
CurrentStateModel::CurrentStateModel() : currentStateModelView(*this), updateTimer(std::chrono::milliseconds{250})
{
    connectionsContainer += updateTimer.onTimeout([this] {
        auto now = std::chrono::system_clock::now();
        for (auto& [key, value] : items) {
            if (!value.connectionLost && (value.lastUpdate + intervalUdpdate) < now) {
                value.connectionLost = true;
                currentStateModelView.connectLostEvent(key);
                currentStateModelView.itemUpdateEvent(key);
            }
        }
    });

    updateTimer.start();
}

CurrentStateModel::~CurrentStateModel()
{
    for (auto it = items.begin(); it != items.end();) {
        auto deviceId = it->first;
        it            = items.erase(it);
        currentStateModelView.itemRemovedEvent(deviceId);
    }
}
const CurrentState* CurrentStateModel::value(DeviceId id) const
{
    auto it = items.find(id);
    if (it == items.end()) {
        return nullptr;
    }
    return &it->second;
}

void CurrentStateModel::setBusy(DeviceId id, bool busy)
{
    auto it = items.find(id);
    if (it == items.end()) {
        return;
    }
    it->second.isBusy = busy;
    currentStateModelView.busyChangedEvent(id);
    currentStateModelView.itemUpdateEvent(id);
}

void CurrentStateModel::insertOrUpdate(std::string ip, Status status)
{
    auto deviceId = status.id;
    auto it       = items.find(deviceId);
    if (it == items.end()) {
        items.emplace(std::piecewise_construct,
                      std::forward_as_tuple(deviceId),
                      std::forward_as_tuple(std::move(status), std::move(ip), std::chrono::system_clock::now()));
        currentStateModelView.itemAddedEvent(deviceId);
    } else {
        auto& cState  = it->second;
        auto& cStatus = cState.status;

        if (cState.ip != ip) {
            auto oldIp = std::move(cState.ip);
            cState.ip  = ip;
            currentStateModelView.ipChangedEvent(deviceId, std::move(oldIp), ip);
        }
        bool updateVectorSlot = (cStatus.relays != status.relays);

        auto oldStatus = std::move(cStatus);
        cStatus        = std::move(status);

        if (oldStatus.uptime > cStatus.uptime) {
            currentStateModelView.rebootDetectEvent(deviceId);
        } else if (oldStatus.wifiInfo.connectionDuration > cStatus.wifiInfo.connectionDuration) {
            currentStateModelView.reconnectDetectedEvent(deviceId);
        }

        if (cState.connectionLost) {
            cState.connectionLost = false;
            currentStateModelView.connectionRestoredEvent(deviceId);
        }

        if (updateVectorSlot) {
            currentStateModelView.relayVectorChangedEvent(deviceId);
        }
        it->second.lastUpdate = std::chrono::system_clock::now();
        ++it->second.numberOfPacketsReceived;
        currentStateModelView.itemUpdateEvent(deviceId);
    }
}

} // namespace barmaley::lib
