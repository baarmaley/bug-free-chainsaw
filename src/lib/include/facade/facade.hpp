#pragma once

#include <common/relay_command.hpp>
#include <common/signals2.hpp>
#include <current_state_model/current_state_model.hpp>
#include <journal/journal_manager_fwd.hpp>
#include <journal/journal_manager_view_fwd.hpp>
#include <receiver/receiver.h>
#include <request_manager/failed_request.hpp>
#include <request_manager/request_manager_fwd.hpp>
#include <request_manager/successful_request.hpp>

#include <memory>
#include <functional>
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

    JournalManagerView& journalManagerView();
    
    void groupCommand(DeviceId id, GroupCommand cmd);
    void singleCommand(DeviceId id, RelayId relayId, SingleCommand cmd);

private:
	void updateModel(std::string ip, std::string data);

    void request(DeviceId id, std::function<std::string(std::string)> urlBuilder);

    void statusRequest(DeviceId id, std::string ip, SuccessfulRequest successfulRequest);
	void errorRequest(DeviceId id, std::string ip, FailedRequest failedRequest);

    Receiver receiver;
    std::unique_ptr<RequestManager> requestManager;
    CurrentStateModel model;
    std::unique_ptr<JournalManager> journalManager;
    ConnectionsContainer connectionsContainer;
};
} // namespace barmaley::lib
