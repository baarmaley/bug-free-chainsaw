#include <journal/journal.hpp>

#include <current_state_model/current_state_model.hpp>
#include <facade/facade_view.hpp>


namespace barmaley::lib {
JournalManager::JournalManager(FacadeView& facadeView, CurrentStateModelView& currentStateModelView)
    : udateTimer(intervalUdpdate)
{
    connectionsContainer += facadeView.onProtocolError([this](std::string ip, std::string errorString) {
        auto it = recentBadProtocolEvents.find(ip);
        if (it != recentBadProtocolEvents.end() && it->second == errorString) {
            return;
        }

        auto addedGuard = beginAddedEntry();
        journal.createEntry<BadProtocolJE>(addedGuard.id, ip, errorString);

        recentBadProtocolEvents.emplace(std::move(ip), std::move(errorString));
    });

    auto currentState = [&currentStateModelView](DeviceId id) {
        return currentStateModelView.currentStateModel.value(id);
    };

    connectionsContainer += currentStateModelView.onAdded([this, currentState](DeviceId id) {
        auto addedGuard = beginAddedEntry();
        auto state      = currentState(id);

        lastStates.emplace(id, *state);
        journal.createEntry<DeviceDetectedJE>(addedGuard.id, id, state->ip);
    });

    connectionsContainer += currentStateModelView.onUpdate([this, currentState](DeviceId id) {
        auto lastStatesIt = lastStates.find(id);
        assert((lastStatesIt != lastStates.end()));
        auto oldState            = lastStatesIt->second;
        auto state               = currentState(id);
        lastStatesIt->second     = LastState(*state);
        const auto& currentState = lastStatesIt->second;

        if (oldState.ip != currentState.ip) {
            auto addedGuard = beginAddedEntry();
            journal.createEntry<IpChangedJE>(addedGuard.id, id, std::move(oldState.ip), currentState.ip);
        }
        if (oldState.uptime > currentState.uptime) {
            auto addedGuard = beginAddedEntry();
            journal.createEntry<RebootDetectJE>(addedGuard.id, id);
        } else if (oldState.connectionDuration > currentState.connectionDuration) {
            auto addedGuard      = beginAddedEntry();
            const auto& wifiInfo = state->status.wifiInfo;
            std::string lastReasonReconnection =
                wifiInfo.lastReasonReconnection ? *wifiInfo.lastReasonReconnection : "";
            journal.createEntry<ReconnectDetectedJE>(
                addedGuard.id, id, wifiInfo.reconnectCount, lastReasonReconnection);
        }

        if (oldState.connectionLost && !currentState.connectionLost) {
            auto addedGuard = beginAddedEntry();
            journal.createEntry<ConnectionRestoredJE>(addedGuard.id, id);
        }
    });

    connectionsContainer += udateTimer.onTimeout([this, currentState = std::move(currentState)] {
        auto intevalSecondsAgo = std::chrono::steady_clock::now() - intervalUdpdate;
        for (auto& [key, value] : lastStates) {
            if (!value.connectionLost && value.lastUpdate < intevalSecondsAgo) {
                auto addedGuard      = beginAddedEntry();
                value.connectionLost = true;
                journal.createEntry<ConnectionLost>(addedGuard.id, key);
            }
        }
    });
    udateTimer.start();
}

JournalManagerView::JournalManagerView(const Journal& journal) : journal(journal) {}
JournalManagerView::~JournalManagerView() = default;
s2::connection JournalManagerView::onAdded(SignalType::slot_type slot)
{
    return itemAddedEvent.connect(std::move(slot));
}
LastState::LastState(const CurrentState& currentState)
    : ip(currentState.ip),
      heap(currentState.status.heap),
      uptime(currentState.status.uptime),
      connectionDuration(currentState.status.wifiInfo.connectionDuration)
{
}
} // namespace barmaley::lib
