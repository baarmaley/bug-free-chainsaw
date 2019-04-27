#pragma once

#include <common/signals2.hpp>
#include <common/relay_command.hpp>
#include <current_state_model/current_state_model.hpp>
#include <facade/facade_view.hpp>
#include <journal/journal.hpp>
#include <receiver/receiver.h>
#include <request_manager/request_manager_fwd.hpp>

#include <memory>
namespace barmaley::lib {
class Facade
{
public:
    Facade();
    ~Facade();

    inline CurrentStateModelView& currentStateModelView()
    {
        return model.currentStateModelView;
    }

    inline JournalManagerView& journalManagerView()
    {
        return journalManager.view();
    }

    void groupCommand(DeviceId id, GroupCommand cmd);
    void singleCommand(DeviceId id, RelayId relayId, SingleCommand cmd);

private:
    FacadeView facadeView;
    Receiver receiver;
    std::unique_ptr<RequestManager> requestManager;
    CurrentStateModel model;
    JournalManager journalManager;
    ConnectionsContainer connectionsContainer;
};
} // namespace barmaley::lib
