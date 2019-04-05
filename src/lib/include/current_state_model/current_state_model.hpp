#pragma once

#include <unordered_map>

#include <common/common_type.hpp>
#include <common/signals2.hpp>

namespace barmaley::lib {

struct CurrentState
{
    CurrentState(Status status, std::string ip, std::chrono::system_clock::time_point lastUpdate)
        : status(std::move(status)), ip(std::move(ip)), lastUpdate(std::move(lastUpdate)){};
    Status status;
    std::string ip;
    std::uint32_t numberOfPacketsReceived = 0;
    std::chrono::system_clock::time_point lastUpdate;
};
class CurrentStateModel;

class CurrentStateModelView
{
public:
    using SignalType = s2::signal<void(DeviceId)>;

    CurrentStateModelView(const CurrentStateModel& currentStateModel);
    CurrentStateModelView(const CurrentStateModelView&) = delete;
    CurrentStateModelView(CurrentStateModelView&&)      = delete;

    ~CurrentStateModelView();

    friend CurrentStateModel;

    s2::connection onAdded(SignalType::slot_type slot);
    s2::connection onUpdate(SignalType::slot_type slot);
    s2::connection onRemoved(SignalType::slot_type slot);

    const CurrentStateModel& currentStateModel;

protected:
    SignalType itemAddedEvent;
    SignalType itemUpdateEvent;
    SignalType itemRemovedEvent;
};

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

    void insertOrUpdate(std::string ip, Status status);

    CurrentStateModelView currentStateModelView;

private:
    ValuesType items;
};

} // namespace barmaley::lib
