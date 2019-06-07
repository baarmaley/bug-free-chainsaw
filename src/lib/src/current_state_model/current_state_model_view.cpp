#include <current_state_model/current_state_model_view.hpp>

namespace barmaley::lib {
CurrentStateModelView::CurrentStateModelView(const CurrentStateModel& currentStateModel)
    : currentStateModel(currentStateModel)
{
}
CurrentStateModelView::~CurrentStateModelView() = default;

s2::connection CurrentStateModelView::onAdded(SignalType::slot_type slot)
{
    return itemAddedEvent.connect(std::move(slot));
}
s2::connection CurrentStateModelView::onUpdate(SignalType::slot_type slot)
{
    return itemUpdateEvent.connect(std::move(slot));
}
s2::connection CurrentStateModelView::onRemoved(SignalType::slot_type slot)
{
    return itemRemovedEvent.connect(std::move(slot));
}

s2::connection CurrentStateModelView::onIpChanged(IpChangedSignalType::slot_type slot)
{
    return ipChangedEvent.connect(std::move(slot));
}

s2::connection CurrentStateModelView::onRelayVectorChanged(SignalType::slot_type slot)
{
    return relayVectorChangedEvent.connect(std::move(slot));
}

s2::connection CurrentStateModelView::onBusyChanged(SignalType::slot_type slot)
{
    return busyChangedEvent.connect(std::move(slot));
}
s2::connection CurrentStateModelView::onConnectLost(SignalType::slot_type slot)
{
    return connectLostEvent.connect(std::move(slot));
}
s2::connection CurrentStateModelView::onConnectionRestored(SignalType::slot_type slot)
{
    return connectionRestoredEvent.connect(std::move(slot));
}

s2::connection CurrentStateModelView::onReconnectDetected(SignalType::slot_type slot)
{
    return reconnectDetectedEvent.connect(std::move(slot));
}

s2::connection CurrentStateModelView::onRebootDetect(SignalType::slot_type slot)
{
    return rebootDetectEvent.connect(std::move(slot));
}

} // namespace barmaley::lib
