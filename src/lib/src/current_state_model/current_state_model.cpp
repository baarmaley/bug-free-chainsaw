#include <current_state_model/current_state_model.hpp>

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

CurrentStateModel::CurrentStateModel() : currentStateModelView(*this) {}

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

void CurrentStateModel::insertOrUpdate(std::string ip, State state, std::optional<Debug> debug)
{
    auto deviceId = state.id;
    auto it       = items.find(deviceId);
    if (it == items.end()) {
        items.emplace(
            std::piecewise_construct,
            std::forward_as_tuple(deviceId),
            std::forward_as_tuple(std::move(state), std::move(debug), std::move(ip), std::chrono::system_clock::now()));
        currentStateModelView.itemAddedEvent(deviceId);
    } else {
        it->second.ip         = ip;
        it->second.state      = std::move(state);
        it->second.lastUpdate = std::chrono::system_clock::now();
        ++it->second.numberOfPacketsReceived;
        currentStateModelView.itemUpdateEvent(deviceId);
    }
}

} // namespace barmaley::lib
