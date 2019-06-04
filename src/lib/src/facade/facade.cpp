#include <facade/facade.hpp>

#include <request_manager/request_manager.hpp>

#include <QDebug>

namespace barmaley::lib {
Facade::Facade()
    : facadeView(*this),
      requestManager(std::make_unique<RequestManager>()),
      journalManager(facadeView, model.currentStateModelView)
{
    connectionsContainer += receiver.onReceived([this] {
        while (!receiver.empty()) {
            auto p = receiver.packet();
            updateModel(std::move(p.ip), std::move(p.data));
        }
    });
}
Facade::~Facade() = default;

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
        facadeView.protocolErrorEvent(ip, e.what());
    }
}
void Facade::request(DeviceId id, std::function<std::string(std::string)> urlBuilder)
{
    model.setBusy(id, true);
    auto ip = model.value(id)->ip;

    requestManager->request(urlBuilder(ip),
                            std::bind(&Facade::statusRequest, this, id, ip, std::placeholders::_1),
                            std::bind(&Facade::errorRequest, this, id, std::placeholders::_1));
}
void Facade::statusRequest(DeviceId id, std::string ip, SuccessfulRequest s)
{
    qDebug() << "Command ok" << QString::fromStdString(s.response);
    auto url = "http://" + ip + "/status/";
    requestManager->request(
        url,
        [id, ip = std::move(ip), this](SuccessfulRequest s) {
            qDebug() << "Status ok:" << QString::fromStdString(s.response);
            updateModel(ip, std::move(s.response));
            model.setBusy(id, false);
        },
        std::bind(&Facade::errorRequest, this, id, std::placeholders::_1));
}

void Facade::errorRequest(DeviceId id, FailedRequest failedRequest)
{
    qDebug() << "command: Error" << QString::fromStdString(failedRequest.what);
    model.setBusy(id, false);
}
} // namespace barmaley::lib
