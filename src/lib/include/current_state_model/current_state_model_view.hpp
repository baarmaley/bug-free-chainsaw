#pragma once

#include <unordered_map>

#include <current_state_model/current_state_model_fwd.hpp>

#include <common/device_id.hpp>
#include <common/signals2.hpp>

namespace barmaley::lib {
class CurrentStateModelView
{
public:
    using SignalType          = s2::signal<void(DeviceId)>;
    using IpChangedSignalType = s2::signal<void(DeviceId, std::string oldIp, std::string newIp)>;

    CurrentStateModelView(const CurrentStateModel& currentStateModel);
    CurrentStateModelView(const CurrentStateModelView&) = delete;
    CurrentStateModelView(CurrentStateModelView&&)      = delete;

    ~CurrentStateModelView();

    friend CurrentStateModel;

    s2::connection onAdded(SignalType::slot_type slot);
    s2::connection onUpdate(SignalType::slot_type slot);
    s2::connection onRemoved(SignalType::slot_type slot);
    s2::connection onIpChanged(IpChangedSignalType::slot_type slot);
    s2::connection onRelayVectorChanged(SignalType::slot_type slot);
    s2::connection onBusyChanged(SignalType::slot_type slot);
    s2::connection onConnectLost(SignalType::slot_type slot);
    s2::connection onConnectionRestored(SignalType::slot_type slot);
    s2::connection onReconnectDetected(SignalType::slot_type slot);
    s2::connection onRebootDetect(SignalType::slot_type slot);

    const CurrentStateModel& currentStateModel;

protected:
    SignalType itemAddedEvent;
    SignalType itemUpdateEvent;
    SignalType itemRemovedEvent;
    IpChangedSignalType ipChangedEvent;
    SignalType relayVectorChangedEvent;
    SignalType busyChangedEvent;
    SignalType connectLostEvent;
    SignalType connectionRestoredEvent;
    SignalType reconnectDetectedEvent;
    SignalType rebootDetectEvent;
};

} // namespace barmaley::lib
