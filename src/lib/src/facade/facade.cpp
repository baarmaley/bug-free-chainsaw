#include <facade/facade.hpp>

#include <journal/journal_manager.hpp>
#include <journal/journal_manager_view.hpp>
#include <request_manager/request_manager.hpp>

#include <QDebug>

namespace barmaley::lib {
namespace {
std::string to_string(lib::DeviceId id)
{
    return std::to_string(toUint32(id));
}
} // namespace
Facade::Facade()
    : requestManager(std::make_unique<RequestManager>()), journalManager(std::make_unique<JournalManager>())
{
    connectionsContainer += receiver.onReceived([this] {
        while (!receiver.empty()) {
            auto p = receiver.packet();
            updateModel(std::move(p.ip), std::move(p.data));
        }
    });
    auto& modelView = model.currentStateModelView;

    connectionsContainer += modelView.onAdded([this](lib::DeviceId id) {
        journalManager->addEntry(JournalEntryType::device_detected, "id: " + to_string(id));
    });

    connectionsContainer += modelView.onIpChanged([this](lib::DeviceId id, std::string oldIp, std::string newIp) {
        journalManager->addEntry(JournalEntryType::ip_changed,
                                 "id: " + to_string(id) + " old ip: " + oldIp + " new ip: " + newIp);
    });

    connectionsContainer += modelView.onConnectionRestored([this](lib::DeviceId id) {
        journalManager->addEntry(JournalEntryType::connection_restored, "id: " + to_string(id));
    });

    connectionsContainer += modelView.onConnectLost([this](lib::DeviceId id) {
        journalManager->addEntry(JournalEntryType::connection_lost, "id: " + to_string(id));
    });

    connectionsContainer += modelView.onRebootDetect([this](lib::DeviceId id) {
        journalManager->addEntry(JournalEntryType::reboot_detect, "id: " + to_string(id));
    });

    connectionsContainer += modelView.onReconnectDetected([this](lib::DeviceId id) {
        journalManager->addEntry(JournalEntryType::reconnect_detected, "id: " + to_string(id));
    });
}
Facade::~Facade() = default;

JournalManagerView& Facade::journalManagerView()
{
    return journalManager->view();
}

void Facade::groupCommand(DeviceId id, GroupCommand cmd)
{
    request(id, [cmd](std::string ip) {
        auto url = "http://" + ip + "/action/";
        switch (cmd) {
            case GroupCommand::on:
                url += "all_on";
                break;
            case GroupCommand::off:
                url += "all_off";
                break;
            default:
                break;
        }
        return url;
    });
}
void Facade::singleCommand(DeviceId id, RelayId relayId, SingleCommand cmd)
{
    request(id, [relayId, cmd](std::string ip) {
        auto url = "http://" + ip + "/action/";
        switch (cmd) {
            case SingleCommand::on:
                url += "on";
                break;
            case SingleCommand::off:
                url += "off";
                break;
            case SingleCommand::inv:
                url += "inversion";
                break;
        }
        url += "/" + std::to_string(toUint32(relayId));
        return url;
    });
}
void Facade::updateModel(std::string ip, std::string data)
{
    try {
        model.insertOrUpdate(ip, json::parse(std::move(data)).at("status"));
    } catch (const json::exception& e) {
        journalManager->addEntry(lib::JournalEntryType::bad_protocol, ip + ": " + e.what());
    }
}
void Facade::request(DeviceId id, std::function<std::string(std::string)> urlBuilder)
{
    model.setBusy(id, true);
    auto ip = model.value(id)->ip;

    requestManager->request(urlBuilder(ip),
                            std::bind(&Facade::statusRequest, this, id, ip, std::placeholders::_1),
                            std::bind(&Facade::errorRequest, this, id, ip, std::placeholders::_1));
}
void Facade::statusRequest(DeviceId id, std::string ip, SuccessfulRequest s)
{
    journalManager->addEntry(lib::JournalEntryType::successful_request, "cmd: " + ip + ": " + s.response);

    auto url = "http://" + ip + "/status/";
    requestManager->request(
        url,
        [id, ip, this](SuccessfulRequest s) {
            journalManager->addEntry(lib::JournalEntryType::successful_request, ip + ": " + s.response);
            updateModel(ip, std::move(s.response));
            model.setBusy(id, false);
        },
        std::bind(&Facade::errorRequest, this, id, ip, std::placeholders::_1));
}

void Facade::errorRequest(DeviceId id, std::string ip, FailedRequest failedRequest)
{
    journalManager->addEntry(lib::JournalEntryType::failed_request, ip + ": " + failedRequest.what);
    model.setBusy(id, false);
}
} // namespace barmaley::lib
