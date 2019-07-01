#pragma once

#include <unordered_map>

#include <current_state_model/current_state.hpp>
#include <current_state_model/current_state_model_view.hpp>

#include <common/common_type.hpp>
#include <common/signals2.hpp>
#include <timer/timer.hpp>

namespace barmaley::lib {

class CurrentStateModel
{
public:
    using ValuesType = std::unordered_map<DeviceId, CurrentState>;

    CurrentStateModel();
    CurrentStateModel(const CurrentStateModel&) = delete;
    CurrentStateModel(CurrentStateModel&&)      = delete;
    ~CurrentStateModel();

    inline const ValuesType& values() const
    {
        return items;
    }

    const CurrentState* value(DeviceId id) const;

    bool hasValue(DeviceId id) const;

    void setBusy(DeviceId id, bool busy);

    void insertOrUpdate(std::string ip, Status status);

    CurrentStateModelView currentStateModelView;

private:
    std::chrono::seconds intervalUdpdate{10};
    Timer updateTimer;
    ValuesType items;
    ConnectionsContainer connectionsContainer;
};

} // namespace barmaley::lib
